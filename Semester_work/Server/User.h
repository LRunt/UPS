/**
 * Class user represents player
 *
 * @author Lukas Runt
 * @date 26-10-2023
 * @version 0.0.1
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

using namespace std;

class User {
    public:
        static vector<std::unique_ptr<User>> users;

        int mState;
        string mUsername;
        /** Number of wrong messages in row, after 3 wrong messages -> kick out */
        int mStrikes;
        bool isConnected;

        User() {
            mStrikes = 0;
            mUsername = "";
	        mState = 0;
            isConnected = true;
            cout << "New user created! State = " << mState << endl;
        }

        int execute_message(string message);

        void disconnect_user();

    private:
        int login(vector<string> parsedMessage);

        void print_existing_users();

        bool exist_user(string username);

        bool user_connected(string username);

        User get_user(string username);

        string toString();
};

