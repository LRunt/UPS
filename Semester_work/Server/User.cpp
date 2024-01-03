/**
 * Methods of user class
 *
 * @author Lukas Runt
 * @date 02-01-2024
 * @version 1.0.0
 */

#include "User.h"

/** Initializing vector of users */
vector<shared_ptr<User>> User::users;

/**
 * Method returns user by file descriptor of null pointer if user is not found
 * @param fd file descriptor
 * @return user by file descriptor of null pointer if user is not found
 */
std::shared_ptr<User> User::get_user_by_fd(int fd){
    for (const auto& userPtr : User::users){
        if (userPtr->mFd == fd){
            return userPtr;
        }
    }
    return nullptr;
}

/**
 * Method printing all users
 */
void User::print_users() {
    Logger::instance().log(LogLevel::INFO, "Printing all users:");
    for (const auto& userPtr : User::users) {
        Logger::instance().log(LogLevel::INFO, userPtr->to_str());
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
    Logger::instance().log(LogLevel::INFO, "Executing message: " + message + ", from client with fd: " + to_string(fd));
    vector<string> parsedMessage = splitString(message);
    if(parsedMessage[0] == MESSAGE_PRINT_USERS && parsedMessage.size() == 1){
        print_users();
    }
    shared_ptr<User> user = find_user_by_fd(fd);
    if(user == nullptr){
        if(parsedMessage[0] == MESSAGE_LOGIN){
            Logger::instance().log(LogLevel::INFO, "Client: " + to_string(fd) + " is trying to login.");
            response = string(MESSAGE_LOGIN) + DELIMITER + to_string(login(parsedMessage, fd));
        }else{
            response = string(MESSAGE_ERROR);
        }
    }else{
        user->mLastPlayerMessage = chrono::high_resolution_clock::now();
        if(parsedMessage[0] == MESSAGE_DISCONNECT){
            Logger::instance().log(LogLevel::INFO, "User: " + user->mUsername + " is trying to logout.");
            user->set_user_disconnected();
        }else{
            int* rematch;
            switch(user->mState){
                case LOGGED:
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
                    if(parsedMessage[0] == MESSAGE_WAITING || parsedMessage[0] == MESSAGE_GAME_STATE || parsedMessage[0] == MESSAGE_PING){
                        if(user->is_game_running()){
                            response = user->mGame->get_game_state(user->mUsername);
                        } else{
                            rematch = user->mGame->get_rematch_state(user->mUsername);
                            response = user->mGame->get_result(user->mUsername, rematch);
                        }
                    }else if(parsedMessage[0] == MESSAGE_MAKE_TURN){
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
                    Logger::instance().log(LogLevel::ERROR, "User: " + user->mUsername + ", has bas state: " + to_string(user->mState));
            }
        }
    }
    return response;
}

/**
 * Sets user as disconnected
 */
void User::set_user_disconnected() {
    logger.log(LogLevel::INFO, "disconnecting user: " + mUsername);
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
 *          6 - Maximal number of users
 *          -1 - Invalid message
 */
int User::login(vector<string> parsedMessage, int fd) {
    if(parsedMessage.size() != 2){
        Logger::instance().log(LogLevel::WARNING, "There are illegal chars in username.");
        return ILLEGAL_CHARACTERS;
    } else{
        std::string username = parsedMessage[1];
        if(username.size() < MIN_USERNAME_LENGTH){
            Logger::instance().log(LogLevel::WARNING, "Username: "  + username + ", is too short.");
            return SHORT_USERNAME;
        }else if(username.size() > MAX_USERNAME_LENGTH){
            Logger::instance().log(LogLevel::WARNING, "Username: "  + username + ", is too long.");
            return LONG_USERNAME;
        }else if(user_exists(username)){
            change_disconnected_users_fd();
            if(user_connected(username)){
                Logger::instance().log(LogLevel::WARNING, "There is online user with same username: " + username);
                return EXIST_ONLINE_USER;
            }else{
                Logger::instance().log(LogLevel::INFO, "There offline account with username: " + username);
                change_user_fd(username, fd);
                return EXIST_OFFLINE_USER;
            }
        }else{
            //adding to the list of users
            users.push_back(std::make_shared<User>(username, fd));
            Logger::instance().log(LogLevel::INFO, "User logged:" + username + ", fd: " + to_string(fd));
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
        logger.log(LogLevel::INFO,"Cannot found a opponent for user: " + this->mUsername);
        return string(MESSAGE_WAITING) + DELIMITER + to_string(this->mWaiting);
    }else{
        logger.log(LogLevel::INFO, "Opponent for user: " + this->mUsername + " found.");
        shared_ptr<Game> new_game = make_shared<Game>(this->mUsername, opponent->mUsername);
        this->mGame = new_game;
        this->mState = IN_GAME;
        this->mWaiting = 0;
        opponent->mGame = new_game;
        opponent->mState = IN_GAME;
        opponent->mWaiting = 0;
        return this->mGame->get_game_state(mUsername);
    }
}

/**
 * Return if the user exists or not
 * @param username nickname what we are looking for
 * @return true - exist, false - do not exist
 */
bool User::user_exists(const std::string& username) {
    Logger::instance().log(LogLevel::INFO, "Searching for user: " + username);
    for (const auto& user : User::users) {
        if (username == user->mUsername) {
            Logger::instance().log(LogLevel::INFO, "User found: " + user->to_str());
            return true;
        }
    }
    Logger::instance().log(LogLevel::INFO, "User with username: " + username + " do not exist.");
    return false;
}

/**
 * Return if the user is online
 * @param username nickname what we are looking for
 * @return true - online, false - offline
 */
bool User::user_connected(const string& username) {
    Logger::instance().log(LogLevel::INFO, "Checking if user: " + username + " is online.");
    for(const auto& user : User::users){
        if(username == user->mUsername){
            if(!connected_by_time(user)){
                Logger::instance().log(LogLevel::INFO, "User: " + username + " is offline.");
                return false;
            }else{
                Logger::instance().log(LogLevel::INFO, "User: " + username + " is online.");
                return true;
            }
        }
    }
    return false;
}

/**
 * Method evaluates whether the user is connected by time
 * @param user user we are testing if he is online
 * @return true - user is online, false - user is not online
 */
bool User::connected_by_time(shared_ptr<User> user){
    chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long> difference = std::chrono::duration_cast<std::chrono::seconds>(now - user->mLastPlayerMessage);
    Logger::instance().log(LogLevel::INFO, "User: " + user->mUsername + " last connection " + to_string(difference.count()) + "s ago.");
    if (difference.count() < DISCONNECTION_TIME) {
        return true;
    }else{
        return false;
    }
}

/**
 * Method checks if users with fd is really connected
 */
void User::change_disconnected_users_fd(){
    for(const auto& user : User::users){
        if(user->mFd != DISCONNECTED && !connected_by_time(user)){
            user->mFd = DISCONNECTED;
        }
    }
}

/**
 * Method find a user by file descriptor
 * @param fd file descriptor
 * @return User_test
 */
shared_ptr<User> User::find_user_by_fd(int fd) {
    Logger::instance().log(LogLevel::INFO, "Searching user with fd: " + to_string(fd));
    for (const auto& user : User::users) {
        if (user->mFd == fd) {
            Logger::instance().log(LogLevel::INFO, "User found: " + user->to_str());
            return user;
        }
    }
    Logger::instance().log(LogLevel::INFO, "User with fd: " + to_string(fd) + ", was not found.");
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
        if (user->mState == state && user->mUsername != username) {
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
    logger.log(LogLevel::INFO, "Evaluating rematch of user: " + mUsername + ", fd(" + to_string(mFd) + ")");
    if(rematch[0] == USER_LOBBY || rematch[0] == BOTH_LOBBY){
        //No rematch
        logger.log(LogLevel::INFO, "User: " + mUsername + " do not want rematch and goes to the lobby.");
        this->mGame = nullptr;
        this->mState = LOGGED;
        return string(MESSAGE_LOGGED);
    }else if(rematch[0] == BOTH_WANT){
        logger.log(LogLevel::INFO, "Both users want rematch.");
        this->mState = IN_GAME;
        this->mGame->reset_game();
        return this->mGame->get_game_state(mUsername);
    }else if(rematch[0] == USER_WANT && rematch[1] == -2 || rematch[0] == OPPONENT_WANT && rematch[1] == -2){
        logger.log(LogLevel::INFO, "Timeout of user response.");
        this->mGame = nullptr;
        this->mState = LOGGED;
        return string(MESSAGE_LOGGED);
    }else{
        logger.log(LogLevel::INFO, "Waiting for users response.");
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


