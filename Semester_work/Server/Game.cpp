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

enum game_state{
    RUNNING = 0,
    WIN_PLAYER1 = 1,
    WIN_PLAYER2 = 2,
    TIE = 3
};

enum game_result{
    WIN = 1,
    LOSE = 2,
    DRAW = 3
};

enum rematch{
    NO_RESPONSE = -1,
    NO = 0,
    YES = 1
};

enum new_game{
    NO_ANSWER = 0,
    OPPONENT_LOBBY = 1,
    OPPONENT_WANT = 2,
    USER_LOBBY = 3,
    USER_WANT = 4,
    BOTH_LOBBY = 5,
    BOTH_WANT = 6
};

#define DELIMITER "|"

#define MESSAGE_GAME_STATE "GAME"
#define MESSAGE_VALID "VALID"
#define MESSAGE_RESULT "RESULT"

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
 * GAME|<side_of_player>|<opponent_name>|<number_of_turn>|<game_status>|<play_field1>|...|<play_fieldN>
 * @param player Player who is asking for game state
 * @return generated message
 */
string Game::get_game_state(const string& player) {
    string response = string(MESSAGE_GAME_STATE) + DELIMITER;
    if(player == mPlayer1){
        response += to_string(X) + DELIMITER + mPlayer2;
    }else{
        response += to_string(O) + DELIMITER + mPlayer1;
    }
    response += DELIMITER + to_string(mTurn);
    response += DELIMITER + to_string(mState);
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
string Game::make_turn(const string &player, int index) {
    int validity = validate_turn(player, index);
    if(validity == VALID){
        //making the turn
        if(mTurn % 2 == 1){
	    mPlayBoard[index] = X;
	}else{
	    mPlayBoard[index] = O;
	}
        mState = check_game_state();
        mTurn++;
        cout << this->to_str() << endl;
        return get_game_state(player);
    } else{
        return string(MESSAGE_VALID) + DELIMITER + to_string(validity);
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
    }else if((player == mPlayer2 && mTurn % 2 != 0) || (player == mPlayer1 && mTurn % 2 != 1)){
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
        if(mPlayBoard[i * 3] == mPlayBoard[i * 3 + 1] && mPlayBoard[i * 3] == mPlayBoard[i * 3 + 2] && mPlayBoard[i * 3] != 0){
            return get_winner_by_turn();
        }
        //Column
        if(mPlayBoard[i] == mPlayBoard[i + 3] && mPlayBoard[i] == mPlayBoard[i + 6] && mPlayBoard[i] != 0){
            return get_winner_by_turn();
        }
    }

    //Check diagonals
    if(mPlayBoard[0] == mPlayBoard[4] && mPlayBoard[4] == mPlayBoard[8] && mPlayBoard[0] != 0){
        return get_winner_by_turn();
    }
    if(mPlayBoard[2] == mPlayBoard[4] && mPlayBoard[4] == mPlayBoard[6] && mPlayBoard[2] != 0){
        return get_winner_by_turn();
    }
    //Testing draw
    if(mTurn == 9){
        return TIE;
    }
    return RUNNING;
}

/**
 * Method return who wins
 * @return who wins
 */
int Game::get_winner_by_turn() const {
    if(this->mTurn % 2 == 0){
        return WIN_PLAYER2;
    }else{
        return WIN_PLAYER1;
    }
}

/**
 * Method set if the user want to rematch or not
 * @param player username of the player
 * @param rematch if the user wants rematch or not
 * @return -1 only one player respond to rematch, 0 - there will be no rematch, 1 - there will be a rematch
 */
int Game::rematch(const string &player, bool rematch) {
    if(player == mPlayer1){
        mRematchP1 = rematch;
    }else{
        mRematchP2 = rematch;
    }
    return get_rematch_state(player);
}

/**
 * Method to get a state of rematch
 * @param player name of the player
 * @return state from enum new_game
 */
int Game::get_rematch_state(const string& player){
    int user_response, opponent_response;
    if(player == mPlayer1){
        user_response = mRematchP1;
        opponent_response = mRematchP2;
    }else{
        user_response = mRematchP2;
        opponent_response = mRematchP1;
    }

    if(user_response == NO_RESPONSE && opponent_response == NO_RESPONSE){
        return NO_ANSWER;
    }else if(user_response == NO_RESPONSE && opponent_response == NO){
        return OPPONENT_LOBBY;
    }else if(user_response == NO_RESPONSE && opponent_response == YES){
        return OPPONENT_WANT;
    }else if(user_response == NO && opponent_response == NO_RESPONSE){
        return USER_LOBBY;
    }else if(user_response == YES && opponent_response == NO_RESPONSE){
        return USER_WANT;
    }else if(user_response == YES && opponent_response == YES){
        return BOTH_WANT;
    }else{
        return BOTH_LOBBY;
    }
}

/**
 * Generating message for result screen
 * @param player the player to whom the message will be sent
 * @return message in format RESULT|<game_result>|<index1>|...|indexN>|<winIndex1>|...|<winIndex3>
 */
string Game::get_result(const string& player){
    string message = string(MESSAGE_RESULT) + DELIMITER + to_string(get_game_result(player));
    for(int field : mPlayBoard){
        message += DELIMITER + to_string(field);
    }
    return message;
}

/**
 * Method returns result of the player
 * @param player the player for whom we are looking for a result
 * @return result of the player
 */
int Game::get_game_result(const string& player){
    if(this->mState == TIE){
        return DRAW;
    }else if((this->mState == WIN_PLAYER1 && player == this->mPlayer1) || (this->mState == WIN_PLAYER2 && player == this->mPlayer2)){
        return WIN;
    }else{
        return LOSE;
    }
}


/**
 * Method to reset game
 */
void Game::reset_game() {
    //Clear play board
    for(int & i : mPlayBoard){
        i = FREE;
    }
    mTurn = 1;
    //setting rematching
    if(mState != 0){
        mState = 0;
    }else{
        mRematchP1 = -1;
        mRematchP2 = -1;
    }
}

/**
 * Text representation of the game
 * @return text representation of instance
 */
string Game::to_str() {
    string text_representation = "Game with Player1: " + mPlayer1 + ", Player2: " + mPlayer2 + "\n";
    for(int i = 0; i < PLAY_BOARD_SIZE; i++){
        text_representation += to_string(mPlayBoard[i]);
        if(i % 3 == 2){
            text_representation += "\n";
        }
    }
    text_representation += "Turn: " + to_string(mTurn) + ", Game state: " + to_string(mState);
    return text_representation;
}



