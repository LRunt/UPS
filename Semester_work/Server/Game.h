/**
 * Class game represents one room of TIC-TAC-TOE
 *
 * @author Lukas Runt
 * @date 03-01-2023
 * @version 1.0.0
 */

#pragma once

#include <iostream>
#include <chrono>
#include "Constants.h"
#include "Enums.h"
#include "Logger.h"

using namespace std;

class Game {

public:

    using TimePoint = std::chrono::high_resolution_clock::time_point;
    /** Array of play fields if hte game */
    int mPlayBoard[PLAY_BOARD_SIZE]{};
    /** Fields where are 3 connected symbols */
    int mWinFields[WIN_COMBINATION_SIZE]{};
    /** Actual turn */
    int mTurn;
    /** State of the game, if the game is running, draw, win...*/
    int mState;
    /** Username of player1 */
    string mPlayer1;
    /** Username of player2 */
    string mPlayer2;
    /** State of player1 answer for rematch */
    int mRematchP1;
    /** State of player2 answer for rematch */
    int mRematchP2;
    /** Time of last message from player1 */
    decltype(chrono::high_resolution_clock::now()) mLastMessageP1;
    /** Time of last message from player2*/
    decltype(chrono::high_resolution_clock::now()) mLastMessageP2;
    /** Reference to the logger */
    Logger& logger = Logger::instance();

    /**
     * Constructor of class Game
     * @param player1 name of player1
     * @param player2 name of player2
     */
    Game(const string& player1, const string& player2){
        mTurn = 1;
        mState = 0;
        mPlayer1 = player1;
        mPlayer2 = player2;
        mLastMessageP1 = chrono::high_resolution_clock::now();
        mLastMessageP2 = chrono::high_resolution_clock::now();
        mRematchP1 = -1;
        mRematchP2 = -1;
        logger.log(LogLevel::INFO, "New game created: " + this->to_str());
    }

    string make_turn(const string& player, int index);

    string get_game_state(const string& player);

    string get_result(const string& player, int* rematch_state);

    int* rematch(const string& player, bool rematch);

    int* get_rematch_state(const string& player);

    void reset_game();

    void set_win_combination(int index1, int index2, int index3);

    int check_opponent_connection(string player);


private:

    void print_board();

    int validate_turn(const string& player, int index);

    int check_game_state();

    int get_game_result(const string& player);

    int get_remaining_time(decltype(chrono::high_resolution_clock::now()) response_time);

    [[nodiscard]] int get_winner_by_turn() const;

    string to_str();

};

