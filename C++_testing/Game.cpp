/**
 * Class game represents one room of TIC-TAC-TOE
 *
 * @author Lukas Runt
 * @date 08-11-2023
 * @version 0.0.1
 */

#include "Game.h"

/**
 * Enumeration of possible field states
 */
enum field_state{
    FREE = 0,
    X = 1,
    O = 2
};

enum turn_code{
    VALID = 0,
    OUT_OF_BOUNDS = 1,
    PLAYING_FIELD_TAKEN = 2,
    INVALID_USER = 3,
    PLAYER_NOT_ON_TURN = 4
};

/**
 * Method prints a play board
 */
void Game::print_board() {
    for(int i = 0; i < PLAY_BOARD_SIZE; i++){
        cout << to_string(mPlayBoard[i]);
        if(i % 3 == 2){
            cout << endl;
        }
    }
}

/**
 * Method to making a turn
 * @param player user who is making a turn
 * @param index where user making turn
 * @return 0 - Turn is valid
 */
int Game::make_turn(const string &player, int index) {
    int validity = validate_turn(player, index);
    if(validity == VALID){
        //making the turn
        mPlayBoard[index] = mTurn;
        if(mTurn == 1){
            mTurn = 2;
        }else{
            mTurn = 1;
        }
        check_game_state();
        return VALID;
    } else{
        return validity;
    }
}

/**
 * Method validating the turn
 * @param player players who making the turn
 * @param index where user making the turn
 * @return code of validity (enum turn_code)
 */
int Game::validate_turn(const string &player, int index) {
    if(index < 0 || index >= PLAY_BOARD_SIZE){
        return OUT_OF_BOUNDS;
    }else if(mPlayBoard[index] != FREE){
        return PLAYING_FIELD_TAKEN;
    }else if(player != mPlayer1 && player != mPlayer2){
        return INVALID_USER;
    }else if((player == mPlayer1 && mTurn != 1) || (player == mPlayer2 && mTurn != 2)){
        return PLAYER_NOT_ON_TURN;
    }else{
        return VALID;
    }
}

/**
 *
 * @return
 */
int Game::check_game_state() {
    return 0;
}

