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
#include "Game.h"
#include "Constants.h"
#include "Enums.h"
#include "Logger.h"

using namespace std;

class User {
public:
    /** List of all users */
    static vector<shared_ptr<User>> users;
    /** State of the user (DISCONNECTED, LOGGED, WAITING, IN_GAME, RESULT_SCREEN)*/
    int mState;
    /** File descriptor of the user (-1 if user is disconnected)*/
    int mFd;
    /** Name of the user */
    string mUsername;
    /** Number of incoming  pings*/
    int mWaiting;
    /** Game what is the user in */
    shared_ptr<Game> mGame;
    /** Reference to the logger */
    Logger& logger = Logger::instance();

    /**
     * Constructor of class User
     * @param name name of the user
     * @param fd file descriptor of the user
     */
    explicit User(const string& name, int fd) {
        mFd = fd;
        mUsername = name;
        mState = 0;
        mWaiting = 0;
        mGame = nullptr;
        logger.log(LogLevel::INFO, "New User " + mUsername + " fd: " + to_string(mFd) + " .");
    }

    static void print_users();

    static shared_ptr<User> get_user_by_fd(int fd);

    static string execute_message(const string& message, int fd);

    void set_user_disconnected();

    static int login(vector<string> parsedMessage, int fd);

    [[nodiscard]] string to_str() const;

private:

    static bool user_exists(const string& username);

    static bool user_connected(const string& username);

    static void change_user_fd(const string& username, int fd);

    string find_user_for_game();

    static shared_ptr<User> find_user_by_fd(int fd);

    static shared_ptr<User> find_user_by_state(int state, const string& username);

    string evaluate_rematch(int* rematch);

    bool is_game_running();
};

