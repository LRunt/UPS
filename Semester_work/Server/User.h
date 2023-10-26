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

class User {
    public:
        int mState;
        std::string mUsername;
        /** Number of wrong messages in row, after 3 wrong messages -> kick out */
        int mStrikes;

        User() {
            std::cout << "New User created" << std::endl;
            mStrikes = 0;
            mUsername = "";
            mState = 0;
        }

        int parse_message(std::string message);

};

