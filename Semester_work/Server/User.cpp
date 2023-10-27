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
vector<unique_ptr<User>> User::users;

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
int User::execute_message(string message) {
    cout << "User send this message:" << message << endl;
    vector<string> parsedMessage = splitString(message);
    cout << mState << endl;
    switch(mState){
        case CONNECTED:
            if(parsedMessage[0] == MESSAGE_LOGIN){
                cout << "User wants to login." << endl;
                this -> login(parsedMessage);
            }else if(parsedMessage[0] == MESSAGE_DISCONNECT){
                cout << "User wants to disconnect." << endl;
            }else{
                cout << "Bad message" << endl;
            }
            break;
        case LOGGED:
            break;
        default:
            cout << "default" << endl;
    }
    return 0;
}

/**
 * Sets user as disconnected
 */
void User::disconnect_user() {
    isConnected = false;
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
int User::login(vector<string> parsedMessage) {
    if(parsedMessage.size() != 2){
        cerr << "Invalid message!" << endl;
        return INVALID_MESSAGE;
    } else{
        if(parsedMessage[1].size() < MIN_USERNAME_LENGTH){
            cerr << "Username is too short" << endl;
            return SHORT_USERNAME;
        }else if(parsedMessage[1].size() > MAX_USERNAME_LENGTH){
            cerr << "Username is too long" << endl;
            return LONG_USERNAME;
        }else{
            int exist = exist_user(parsedMessage[1]);
            if(exist == -1){
                mUsername = parsedMessage[1] + '\0';
                mState++;
                //adding to the list of users
                users.push_back(std::make_unique<User>(*this));
                cout << "User logged with username: " << mUsername << endl;
                cout << "List of all Users: " << endl;
                print_existing_users();
                return NEW_USER;
            }else if(exist == 0){
                cout << "welcome back!" << endl;
                //TODO: implement loading user state
                return EXIST_OFFLINE_USER;
            }else{
                // state is > 0
                cout << "Error: exist user with same username!" << endl;
                return EXIST_ONLINE_USER;
            }
        }
    }
}

/**
 * Method printing all existing users to the command line
 */
void User::print_existing_users() {
    for(const auto& user : User::users){
        cout << user -> mUsername << endl;
    }
}

/**
 * Return if the user exists or not
 * @param username nickname what we are looking for
 * @return 0 - do not exist, -1 - disconnected, 0 < n - user is online
 */
int User::exist_user(string username) {
    for(const auto& user : User::users){
        if(username == user->mUsername){
            return user->isConnected;
        }
    }
    return -1;
}

/**
 * String representation of User
 * @return string representation of user
 */
string User::toString() {
    return "User: " + mUsername + ", state: " + to_string(mState) + ", is connected: " + to_string(isConnected);
}
