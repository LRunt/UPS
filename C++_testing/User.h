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

using namespace std;

class User {
public:
    int mState;
    string mUsername;
    /** Number of wrong messages in row, after 3 wrong messages -> kick out */
    int mStrikes;

    User() {
        mStrikes = 0;
        mUsername = "";
        mState = 0;
        cout << "New user created! State = " << mState << endl;
    }

    int execute_message(string message);
};
