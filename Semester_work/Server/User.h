//
// Created by Lenovo on 22.10.2023.
//

#pragma once

#include <iostream>
#include <string>

class User {
    public:
        int mState;
        std::string mUsername;
        /** Number of wrong messages in row, after 3 wrong messages -> kick out */
        int mStrikes;

        explicit User() {
            std::cout << "New User created" << std::endl;
            mStrikes = 0;
            mUsername = nullptr;
            mState = 0;
        }

};

