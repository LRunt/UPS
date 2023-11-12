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

enum game_code{
    RUNNING = 0,
    WIN = 1,
    DRAW = 2
};

#define DELIMITER "|"

#define GAME_STATE_MESSAGE "GAME"

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
 * Generating message about state of the game in format:
 * GAME|<side_of_player>|<opponent_name>|<number_of_turn>|<play_field1>|...|<play_fieldN>
 * @param player Player who is asking for game state
 * @return generated message
 */
string Game::get_game_state(const string& player) {
    string response = string(GAME_STATE_MESSAGE) + DELIMITER;
    if(player == mPlayer1){
        response += to_string(X) + DELIMITER + mPlayer2;
    }else{
        response += to_string(O) + DELIMITER + mPlayer1;
    }
    response += DELIMITER + to_string(mTurn);
    for(int i : mPlayBoard){
        response += DELIMITER + to_string(i);
    }
    return response;
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
        mTurn++;
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
    }else if((player == mPlayer2 && mTurn % 2 != 1) || (player == mPlayer1 && mTurn % 2 != 0)){
        return PLAYER_NOT_ON_TURN;
    }else{
        return VALID;
    }
}

/**
 * Method checks if someone win, or its a draw
 * @return state of game
 */
int Game::check_game_state() {
    //Check columns and rows
    for(int i = 0; i < 3; i++){
        //Row
        if(mPlayBoard[i * 3] == mPlayBoard[i * 3 + 1] && mPlayBoard[i * 3] == mPlayBoard[i * 3 + 2] && mPlayBoard[i] != 0){
            return WIN;
        }
        //Column
        if(mPlayBoard[i] == mPlayBoard[i + 3] && mPlayBoard[i] == mPlayBoard[i + 6] && mPlayBoard[i] != 0){
            return WIN;
        }
    }

    //Check diagonals
    if(mPlayBoard[0] == mPlayBoard[4] && mPlayBoard[4] == mPlayBoard[8] && mPlayBoard[0] != 0){
        return WIN;
    }
    if(mPlayBoard[2] == mPlayBoard[4] && mPlayBoard[4] == mPlayBoard[6] && mPlayBoard[2] != 0){
        return WIN;
    }
    //Testing draw
    if(mTurn == 9){
        return DRAW;
    }
    return RUNNING;
}

