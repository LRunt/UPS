/**
 * Methods of user class
 *
 * @author Lukas Runt
 * @date 26-10-2023
 * @version 0.0.1
 */

#include "User.h"

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
            mUsername = parsedMessage[1] + '\0';
            mState++;
            cout << "User logged with username: " << mUsername << endl;
            return NEW_USER;
        }
    }
    return INVALID_MESSAGE;
}
