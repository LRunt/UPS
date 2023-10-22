//
// Created by Lenovo on 22.10.2023.
//

#pragma once

#include <iostream>
#include <string>

class User {
    public:
        int state;

        explicit User() {
            std::cout << "New User created" << std::endl;
            mStrikes = 0;
            mUsername = nullptr;
            state = 0;
        }

    private:
        std::string mUsername;
        /** Number of wrong messages in row, after 3 wrong messages -> kick out */
        int mStrikes;

};

