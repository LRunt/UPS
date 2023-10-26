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
    cout << "User received this message:" << message << endl;
    vector<string> parsedMessage = splitString(message);
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
