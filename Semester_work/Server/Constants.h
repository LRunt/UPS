/**
 * Header contains all constants of the application
 *
 * @author Lukas Runt
 * @date 02-01-2024
 * @version 1.0.0
 */

#pragma once

#include <string>

using namespace std;

// DEFAULT INITIALIZATION
const int MAX_BUFFER_SIZE = 1024;
const int DEFAULT_PORT = 10000;
const int MINIMAL_PORT_NUMBER = 1024;
const int MAXIMAL_PORT_NUMBER = 49151;
const int MINIMAL_NUMBER_OF_USERS = 2;
const int DEFAULT_MAX_USERS = 10;
const int NUMBER_OF_STREAMS = 4; //(stdin,stdout, stderr, server)
const string NAME_OF_LOGFILE = "logfile.log";

/** end of message */
const char END_OF_MESSAGE = '\n';
/** delimiter in incoming messages */
const char DELIMITER = '|';
/** 30 seconds */
const int WAITING_TIME_FOR_OPPONENT = 30;
const int DISCONNECTION_TIME = 1;

const int MIN_USERNAME_LENGTH = 3;
const int MAX_USERNAME_LENGTH = 20;

// Play board
const int PLAY_BOARD_SIZE = 9;
const int WIN_COMBINATION_SIZE = 3;

// MESSAGES
const string MESSAGE_PING = "PING";
const string MESSAGE_ERROR = "ERROR";
const string MESSAGE_GAME_STATE = "GAME";
const string MESSAGE_VALID = "VALID";
const string MESSAGE_RESULT = "RESULT";
const string MESSAGE_LOGIN = "LOGIN";
const string MESSAGE_DISCONNECT = "DISCONNECT";
const string MESSAGE_START_SEARCHING_GAME = "START";
const string MESSAGE_CANCEL_SEARCHING_GAME = "CANCEL";
const string MESSAGE_WAITING = "WAITING";
const string MESSAGE_MAKE_TURN = "TURN";
const string MESSAGE_REMATCH = "REMATCH";
const string MESSAGE_LOGGED = "LOGGED";
const string MESSAGE_MAX_USERS = "LOGIN|6\n";
const string MESSAGE_EXIST_ONLINE_USER = "LOGIN|2";
const string MESSAGE_ILLEGAL_CHARS_IN_USERNAME = "LOGIN|3";
const string MESSAGE_SHORT_USERNAME = "LOGIN|4";
const string MESSAGE_LONG_USERNAME = "LOGIN|5";
const string MESSAGE_PRINT_USERS = "USERS";
const string MESSAGE_NEW_LINE = "NEW_LINE";
