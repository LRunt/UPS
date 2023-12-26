/**
 * Class game represents one room of TIC-TAC-TOE
 *
 * @author Lukas Runt
 * @date 08-11-2023
 * @version 0.0.1
 */

#pragma once

#include <iostream>
#include <chrono>

using namespace std;

class Game {

public:
    static const int PLAY_BOARD_SIZE = 9;

    static const int WIN_COMBINATION_SIZE = 3;

    int mPlayBoard[PLAY_BOARD_SIZE]{};

    int mWinFields[WIN_COMBINATION_SIZE]{};

    int mTurn;

    int mState;

    string mPlayer1;

    string mPlayer2;

    int mRematchP1;

    int mRematchP2;

    using TimePoint = std::chrono::high_resolution_clock::time_point;

    decltype(chrono::high_resolution_clock::now()) mLastMessageP1;
    decltype(chrono::high_resolution_clock::now()) mLastMessageP2;

    Game(const string& player1, const string& player2){
        cout << "New game created!" << endl;
        mTurn = 1;
        mState = 0;
        mPlayer1 = player1;
        mPlayer2 = player2;
        mLastMessageP1 = chrono::high_resolution_clock::now();;
        mLastMessageP2 = chrono::high_resolution_clock::now();;
        mRematchP1 = -1;
        mRematchP2 = -1;
        cout << this->to_str() << endl;
    }

    string make_turn(const string& player, int index);

    string get_game_state(const string& player);

    string get_result(const string& player, int rematch_state);

    int rematch(const string& player, bool rematch);

    int get_rematch_state(const string& player);

    void reset_game();

    void set_win_combination(int index1, int index2, int index3);

    int check_opponent_connection(string player);


private:

    void print_board();

    int validate_turn(const string& player, int index);

    int check_game_state();

    int get_game_result(const string& player);

    [[nodiscard]] int get_winner_by_turn() const;

    string to_str();

};

