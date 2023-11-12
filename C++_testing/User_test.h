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
#include "Game_test.h"

using namespace std;

class User_test {
public:
    static vector<shared_ptr<User_test>> users;

    int mState;
    int mFd;
    string mUsername;
    /** Number of wrong messages in row, after 3 wrong messages -> kick out */
    int mStrikes;
    shared_ptr<Game_test> mGame;

    explicit User_test(const string& name, int fd) {
        mFd = fd;
        mStrikes = 0;
        mUsername = name;
        mState = 0;
        mGame = nullptr;
        cout << "New user created! State = " << mState << endl;
    }

    static void print_users();

    static shared_ptr<User_test> get_user_by_fd(int fd);

    static string execute_message(const string& message, int fd);

    void disconnect_user();

    static int login(vector<string> parsedMessage, int fd);

    [[nodiscard]] string to_str() const;

private:

    static bool user_exists(const string& username);

    static bool user_connected(const string& username);

    static void change_user_fd(const string& username, int fd);

    string find_user_for_game();

    static shared_ptr<User_test> find_user_by_fd(int fd);

    static shared_ptr<User_test> find_user_by_state(int state, const string& username);

    string evaluate_rematch(int rematch);

    void test_if_game_is_running();
};

