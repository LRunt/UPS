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

#define MIN_USERNAME_LENGTH 3
#define MAX_USERNAME_LENGTH 20

/**
 * Defining expected messages
 */
#define MESSAGE_LOGIN "LOGIN"
#define MESSAGE_DISCONNECT "DISCONNECT"
#define MESSAGE_START_SEARCHING_GAME "START"
#define MESSAGE_CANCEL_SEARCHING_GAME "STORNO"
#define MESSAGE_MAKE_TURN "TURN"

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
        }else{
            cout << "Bad message" << endl;
        }
    }else{
        if(parsedMessage[0] == MESSAGE_DISCONNECT){
            cout << "User wants to disconnect." << endl;
            user->disconnect_user();
        }else{
            switch(user->mState){
                case LOGGED:
                    cout <<"Logged" << endl;
                    if(parsedMessage[0] == MESSAGE_START_SEARCHING_GAME){
                        user->mState++;
                        user->find_user_for_game();
                    }
                    break;
                case WAITING:
                    if(parsedMessage[0] == MESSAGE_CANCEL_SEARCHING_GAME){
                        user->mState = LOGGED;
                    }
                    cout << "Waiting" << endl;
                    break;
                case IN_GAME:
                    if(parsedMessage[0] == MESSAGE_MAKE_TURN){
                        user->mGame->make_turn(user->mUsername, stoi(parsedMessage[1]));
                    }
                    cout << "in game" << endl;
                    break;
                case RESULT_SCREEN:
                    cout <<"result screen" << endl;
                    break;
                default:
                    cout << "default" << endl;
            }
        }
    }
    return response;
}

/**
 * Sets user as disconnected
 */
void User::disconnect_user() {
    cout << "disconnecting user" << endl;
    mFd = -1;
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
            cout << "User logged with username: " << username << endl;
            cout << "List of all Users: " << endl;
            return NEW_USER;
        }
    }
}

/**
 * Method try to find opponent of the player
 * @return true - opponent founded, false - opponent not found
 */
bool User::find_user_for_game() {
    shared_ptr<User> opponent = find_user_by_state(WAITING, this->mUsername);
    if(opponent == nullptr){
        cout << "Opponent not found" << endl;
        return false;
    }else{
        shared_ptr<Game> new_game = make_shared<Game>(this->mUsername, opponent->mUsername);
        this->mGame = new_game;
        this->mState = IN_GAME;
        opponent->mGame = new_game;
        opponent->mState = IN_GAME;
        cout << "Game created! Player1: " << this->mUsername << ", Player2: " << opponent->mUsername << endl;
        return true;
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
 * @return User
 */
shared_ptr<User> User::find_user_by_fd(int fd) {
    for (const auto& user : User::users) {
        if (user->mFd == fd) {
            return user;
        }
    }
    return nullptr; // User not found
}

/**
 * Method finds a user by his state and it not the same with the username
 * @param state state of the user
 * @param username username what user can has
 * @return User
 */
shared_ptr<User> User::find_user_by_state(int state, const string& username) {
    for (const auto& user : User::users) {
        if (user->mState == state && user->mUsername != username) {
            return user;
        }
    }
    return nullptr; // User not found
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
 * String representation of User
 * @return string representation of user
 */
string User::to_str() const {
    return "User: " + mUsername + ", state: " + to_string(mState) + ", fd: " + to_string(mFd);
}


