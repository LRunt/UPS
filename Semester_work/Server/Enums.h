/**
 * Header contains all enumerations of the application
 *
 * @author Lukas Runt
 * @date 02-01-2024
 * @version 1.0.0
 */


#pragma once

/**
 * Enumeration of possible states
 */
enum states{
    DISCONNECTED = -1,
    LOGGED = 0,
    WAITING = 1,
    IN_GAME = 2,
    RESULT_SCREEN = 3
};

/**
 * Enumerate of returning codes
 */
enum login_code{
    INVALID_MESSAGE = -1,
    NEW_USER = 0,
    EXIST_OFFLINE_USER = 1,
    EXIST_ONLINE_USER = 2,
    ILLEGAL_CHARACTERS = 3,
    SHORT_USERNAME = 4,
    LONG_USERNAME = 5
};

/**
 * Enumeration of rematch/new game
 */
enum new_game{
    NO_ANSWER = 0,
    OPPONENT_LOBBY = 1,
    OPPONENT_WANT = 2,
    USER_LOBBY = 3,
    USER_WANT = 4,
    BOTH_LOBBY = 5,
    BOTH_WANT = 6
};

/**
 * Enumeration of possible field states
 */
enum field_state{
    FREE = 0,
    X = 1,
    O = 2
};

/**
 * Enumeration of returning codes of user turn
 */
enum turn_code{
    VALID = 0,
    OUT_OF_BOUNDS = 1,
    PLAYING_FIELD_TAKEN = 2,
    INVALID_USER = 3,
    PLAYER_NOT_ON_TURN = 4
};

/**
 * Enumeration of game states from player point of view
 */
enum game_state{
    RUNNING = 0,
    WIN_PLAYER1 = 1,
    WIN_PLAYER2 = 2,
    TIE = 3,
    USER_DISCONNECT = 4
};

/**
 * Enumeration of game results
 */
enum game_result{
    WIN = 1,
    LOSE = 2,
    DRAW = 3,
    OPPONENT_DISCONNECT = 4
};

/**
 * Enumeration of rematch responses
 */
enum rematch{
    NO_RESPONSE = -1,
    NO = 0,
    YES = 1
};

/**
 * Enumeration of timeout
 */
enum rematch_timeout {
    NO_TIMEOUT = -1,
    TIME_IS_UP = -2
};



