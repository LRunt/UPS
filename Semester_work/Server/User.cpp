/**
 * Methods of user class
 *
 * @author Lukas Runt
 * @date 27-10-2023
 * @version 0.0.2
 */

#include "User.h"

/** delimiter in incoming messages */
#define DELIMITER '|'

/**
 * Enumeration of possible states
 */
enum states{
    DISCONNECTED = -1,
    LOGGED = 0,
    WAITING = 1,
    IN_GAME = 2,
    RESULT_SCREEN = 3
};

/**
 * Enumerate of returning codes
 */
enum login_code{
    INVALID_MESSAGE = -1,
    NEW_USER = 0,
    EXIST_OFFLINE_USER = 1,
    EXIST_ONLINE_USER = 2,
    ILLEGAL_CHARACTERS = 3,
    SHORT_USERNAME = 4,
    LONG_USERNAME = 5
};

enum new_game{
    NO_ANSWER = 0,
    OPPONENT_LOBBY = 1,
    OPPONENT_WANT = 2,
    USER_LOBBY = 3,
    USER_WANT = 4,
    BOTH_LOBBY = 5,
    BOTH_WANT = 6
};

#define MIN_USERNAME_LENGTH 3
#define MAX_USERNAME_LENGTH 20

/**
 * Defining expected messages
 */
#define MESSAGE_LOGIN "LOGIN"
#define MESSAGE_DISCONNECT "DISCONNECT"
#define MESSAGE_START_SEARCHING_GAME "START"
#define MESSAGE_CANCEL_SEARCHING_GAME "STORNO"
#define MESSAGE_WAITING "WAITING"
#define MESSAGE_MAKE_TURN "TURN"
#define MESSAGE_GAME_STATUS "GAME"
#define MESSAGE_REMATCH "REMATCH"
#define MESSAGE_ERROR "ERROR"
#define MESSAGE_LOGGED "LOGGED"
#define MESSAGE_PING "PING"

/** Initializing vector of users */
vector<shared_ptr<User>> User::users;

std::shared_ptr<User> User::get_user_by_fd(int fd){
    for (const auto& userPtr : User::users){
        if (userPtr->mFd == fd){
            return userPtr;
        }
    }
    return nullptr;
}

void User::print_users() {
    for (const auto& userPtr : User::users) {
        cout << userPtr->to_str() << endl;
    }
}

/**
 * Method split message by delimiter and returns parsed vector
 * @param text message to be parsed
 * @return vector of message and parameters
 */
vector<string> splitString(const string& text){
    vector<string> splitString;
    istringstream iss(text);
    string token;
    while(getline(iss, token, DELIMITER)){
        if (!token.empty())
            splitString.push_back(token);
    }
    return splitString;
}

/**
 * Method gets message and do the think in message
 * @param message message from client
 * @return Response for the message
 */
string User::execute_message(const string& message, int fd) {
    string response;
    cout << "Client" << fd << " send this message:" << message << endl;
    vector<string> parsedMessage = splitString(message);
    shared_ptr<User> user = find_user_by_fd(fd);
    if(user == nullptr){
        if(parsedMessage[0] == MESSAGE_LOGIN){
            cout << "User wants to login." << endl;
            response = string(MESSAGE_LOGIN) + DELIMITER + to_string(login(parsedMessage, fd));
	    User::print_users();
        }else{
            response = string(MESSAGE_ERROR);
            cout << "Error in message: " << message << endl;
        }
    }else{
        if(parsedMessage[0] == MESSAGE_DISCONNECT){
            cout << "User wants to disconnect." << endl;
            user->set_user_disconnected();
        }else{
            int* rematch;
            switch(user->mState){
                case LOGGED:
                    cout <<"Logged" << endl;
                    if(parsedMessage[0] == MESSAGE_PING){
                        response = MESSAGE_LOGGED;
                    }else if(parsedMessage[0] == MESSAGE_START_SEARCHING_GAME){
                        user->mState++;
                        response = user->find_user_for_game();
                    }else{
                        response = string(MESSAGE_ERROR);
                    }
                    break;
                case WAITING:
                    cout << "Waiting" << endl;
                    if(parsedMessage[0] == MESSAGE_PING || parsedMessage[0] == MESSAGE_WAITING){
                        response = string(MESSAGE_WAITING) + DELIMITER + to_string(user->mWaiting);
                        user->mWaiting++;
                    }else if(parsedMessage[0] == MESSAGE_CANCEL_SEARCHING_GAME){
                        user->mState = LOGGED;
                        user->mWaiting = 0;
                        response = string(MESSAGE_CANCEL_SEARCHING_GAME);
                    }else{
                        response = string(MESSAGE_ERROR);
                    }
                    break;
                case IN_GAME:
                    cout << "In game" << endl;
                    if(parsedMessage[0] == MESSAGE_WAITING || parsedMessage[0] == MESSAGE_GAME_STATUS || parsedMessage[0] == MESSAGE_PING){
                        if(user->is_game_running()){
                            response = user->mGame->get_game_state(user->mUsername);
                        } else{
                            rematch = user->mGame->get_rematch_state(user->mUsername);
                            response = user->mGame->get_result(user->mUsername, rematch);
                        }
                    }else if(parsedMessage[0] == MESSAGE_MAKE_TURN){
                        cout << "making turn" << endl;
                        response = user->mGame->make_turn(user->mUsername, stoi(parsedMessage[1]));
                        //if game ended
                        if(!user->is_game_running()){
                            rematch = user->mGame->get_rematch_state(user->mUsername);
                            response = user->mGame->get_result(user->mUsername, rematch);
                        }
                    }else{
                        response = string(MESSAGE_ERROR);
                    }
                    break;
                case RESULT_SCREEN:
                    cout <<"Result screen" << endl;
                    if(parsedMessage[0] == MESSAGE_PING || parsedMessage[0] == MESSAGE_WAITING){
                        rematch = user->mGame->get_rematch_state(user->mUsername);
                        response = user->evaluate_rematch(rematch);
                    }else if(parsedMessage[0] == MESSAGE_REMATCH){
                        rematch = user->mGame->rematch(user->mUsername, stoi(parsedMessage[1]));
                        response = user->evaluate_rematch(rematch);
                    }else{
                        response = string(MESSAGE_ERROR);
                    }

                    break;
                default:
                    response = string(MESSAGE_ERROR);
                    cout << "User have bad state" << endl;
            }
        }
    }
    return response;
}

/**
 * Sets user as disconnected
 */
void User::set_user_disconnected() {
    cout << "disconnecting user" << endl;
    mFd = DISCONNECTED;
}

/**
 * Method try to login the user
 * @param parsedMessage message form client
 * @return Code if the action was successful
 *          0 - Logged new user
 *          1 - Exist offline user with same username
 *          2 - Exist online user with same username
 *          3 - Username contains illegal characters
 *          4 - Username is short
 *          5 - Username is too long
 *          -1 - Invalid message
 */
int User::login(vector<string> parsedMessage, int fd) {
    if(parsedMessage.size() != 2){
        cerr << "Invalid message!" << endl;
        return INVALID_MESSAGE;
    } else{
        std::string username = parsedMessage[1];
        if(username.size() < MIN_USERNAME_LENGTH){
            cerr << "Username is too short" << endl;
            return SHORT_USERNAME;
        }else if(username.size() > MAX_USERNAME_LENGTH){
            cerr << "Username is too long" << endl;
            return LONG_USERNAME;
        }else if(user_exists(username)){
            if(user_connected(username)){
                cout << "Error: exist online user with same username!" << endl;
                return EXIST_ONLINE_USER;
            }else{
                cout << "welcome back!" << endl;
                change_user_fd(username, fd);
                return EXIST_OFFLINE_USER;
            }
        }else{
            //adding to the list of users
            users.push_back(std::make_shared<User>(username, fd));
            cout << "User_test logged with username: " << username << endl;
            cout << "List of all Users: " << endl;
            return NEW_USER;
        }
    }
}

/**
 * Method try to find opponent of the player
 * @return GAME message - opponent found, WAITING - opponent not found
 */
string User::find_user_for_game() {
    shared_ptr<User> opponent = find_user_by_state(WAITING, this->mUsername);
    if(opponent == nullptr){
        cout << "Opponent not found" << endl;
        return string(MESSAGE_WAITING) + DELIMITER + to_string(this->mWaiting);
    }else{
        shared_ptr<Game> new_game = make_shared<Game>(this->mUsername, opponent->mUsername);
        this->mGame = new_game;
        this->mState = IN_GAME;
        this->mWaiting = 0;
        opponent->mGame = new_game;
        opponent->mState = IN_GAME;
        opponent->mWaiting = 0;
        cout << "Game_test created! Player1: " << this->mUsername << ", Player2: " << opponent->mUsername << endl;
        return this->mGame->get_game_state(mUsername);
    }
}

/**
 * Return if the user exists or not
 * @param username nickname what we are looking for
 * @return true - exist, false - do not exist
 */
bool User::user_exists(const std::string& username) {
    for (const auto& user : User::users) {
        if (username == user->mUsername) {
            return true;
        }
    }
    return false;
}

/**
 * Return if the user is online
 * @param username nickname what we are looking for
 * @return true - online, false - offline
 */
bool User::user_connected(const string& username) {
    for(const auto& user : User::users){
        if(username == user->mUsername){
            if(user -> mFd == -1){
                return false;
            }else{
                return true;
            }
        }
    }
    return false;
}

/**
 * Method find a user by file descriptor
 * @param fd file descriptor
 * @return User_test
 */
shared_ptr<User> User::find_user_by_fd(int fd) {
    for (const auto& user : User::users) {
        if (user->mFd == fd) {
            return user;
        }
    }
    return nullptr; // User_test not found
}

/**
 * Method finds a user by his state and it not the same with the username
 * @param state state of the user
 * @param username username what user can has
 * @return User_test
 */
shared_ptr<User> User::find_user_by_state(int state, const string& username) {
    for (const auto& user : User::users) {
        if (user->mState == state && user->mUsername != username && user->mFd != DISCONNECTED) {
            return user;
        }
    }
    return nullptr; // User_test not found
}

/**
 * Method change user file descriptor
 * @param username user username
 * @param fd file descriptor that will be writen to the user
 */
void User::change_user_fd(const string &username, int fd) {
    for (const auto& user : User::users) {
        if(user->mUsername == username){
            user->mFd = fd;
            break;
        }
    }
}

/**
 * Method test if game is running. If not then set user state to game result screen.
 * @return true - game is running, false - game is not running
 */
bool User::is_game_running() {
    if(this->mGame->mState != 0){
        this->mState = RESULT_SCREEN;
        return false;
    }
    return true;
}

/**
 * Method evaluate the rematch and do everything possible
 * @param rematch rematch code 0-4 = waiting for response of both users, 5 = no_rematch, 6 = rematch
 * @return rematch string
 */
string User::evaluate_rematch(int* rematch) {
    if(rematch[0] == USER_LOBBY || rematch[0] == BOTH_LOBBY){
        //No rematch
        this->mGame = nullptr;
        this->mState = LOGGED;
        return string(MESSAGE_LOGGED);
    }else if(rematch[0] == BOTH_WANT){
        this->mState = IN_GAME;
        this->mGame->reset_game();
        return this->mGame->get_game_state(mUsername);
    }else{
        return this->mGame->get_result(mUsername, rematch);
    }
}

/**
 * String representation of User_test
 * @return string representation of user
 */
string User::to_str() const {
    return "User: " + mUsername + ", state: " + to_string(mState) + ", fd: " + to_string(mFd);
}


