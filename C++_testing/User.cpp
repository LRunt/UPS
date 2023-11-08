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
    CONNECTED = 0,
    LOGGED = 1,
    WAITING = 2,
    IN_GAME = 3,
    RESULT_SCREEN = 4
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
#define MESSAGE_START_GAME "START"

/** Initializing vector of users */
vector<shared_ptr<User>> User::users;

std::shared_ptr<User> User::getUserByFd(int fd){
    for (const auto& userPtr : User::users){
        if (userPtr->mFd == fd){
            return userPtr;
        }
    }
    return nullptr;
}

void User::printUsers() {
    for (const auto& userPtr : User::users) {
        cout << userPtr->toString() << endl;
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
 * @return Code if the action was successful
 */
int User::execute_message(const string& message, int fd) {
    cout << "Client" << fd << " send this message:" << message << endl;
    vector<string> parsedMessage = splitString(message);
    if(find_user_by_fd(fd) == nullptr){
        if(parsedMessage[0] == MESSAGE_LOGIN){
            cout << "User wants to login." << endl;
            this -> login(parsedMessage, fd);
        }else if(parsedMessage[0] == MESSAGE_DISCONNECT){
            cout << "User wants to disconnect." << endl;
        }else{
            cout << "Bad message" << endl;
        }
    }else{
        switch(mState){
            break;
        case LOGGED:
            break;
        default:
            cout << "default" << endl;
        }
    }
    return 0;
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
                //TODO: implement loading user state
                return EXIST_OFFLINE_USER;
            }
        }else{
            mUsername = username;
            mState++;
            //adding to the list of users
            users.push_back(std::make_shared<User>(*this));
            cout << "User logged with username: " << mUsername << endl;
            cout << "List of all Users: " << endl;
            return NEW_USER;
        }
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
            return user -> mFd;
        }
    }
    return false;
}

/**
 * String representation of User
 * @return string representation of user
 */
string User::toString() const {
    return "User: " + mUsername + ", state: " + to_string(mState) + ", is connected: " + to_string(mFd);
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
