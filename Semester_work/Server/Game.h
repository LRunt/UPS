/**
 * Class game represents one room of TIC-TAC-TOE
 *
 * @author Lukas Runt
 * @date 08-11-2023
 * @version 0.0.1
 */

#pragma once

#include <iostream>

using namespace std;

class Game {

public:
    static const int PLAY_BOARD_SIZE = 9;

    int mPlayBoard[PLAY_BOARD_SIZE]{};

    int mTurn;

    int mState;

    string mPlayer1;

    string mPlayer2;

    int mRematchP1;

    int mRematchP2;

    Game(const string& player1, const string& player2){
        cout << "New game created!" << endl;
        print_board();
        mTurn = 1;
        mState = 0;
        mPlayer1 = player1;
        mPlayer2 = player2;
        mRematchP1 = -1;
        mRematchP2 = -1;
    }

    string make_turn(const string& player, int index);

    string get_game_state(const string& player);

    int rematch(const string& player, bool rematch);

    [[nodiscard]] int get_rematch_state() const;

    void reset_game();


private:

    void print_board();

    int validate_turn(const string& player, int index);

    int check_game_state();

    [[nodiscard]] int get_winner_by_turn() const;

    string to_str();

};

