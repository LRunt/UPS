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
#include <utility>
#include <vector>
#include <sstream>
#include <memory>

using namespace std;

class User {
public:
    static vector<shared_ptr<User>> users;

    int mState;
    int mFd;
    string mUsername;
    /** Number of wrong messages in row, after 3 wrong messages -> kick out */
    int mStrikes;

    explicit User(int fd) {
        mFd = fd;
        mStrikes = 0;
        mUsername = "";
        mState = 0;
        cout << "New user created! State = " << mState << endl;
    }

    static void printUsers();

    static std::shared_ptr<User> getUserByFd(int fd);

    int execute_message(const string& message, int fd);

    void disconnect_user();

    [[nodiscard]] string toString() const;

private:
    int login(vector<string> parsedMessage, int fd);

    static bool exist_user(const string& username);

    static bool user_connected(const string& username);

    User get_user(string username);


};

