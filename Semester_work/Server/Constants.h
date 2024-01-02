//
// Created by Lenovo on 31.12.2023.
//

#pragma once

#include <string>

using namespace std;

// DEFAULT INITIALIZATION
const int MAX_BUFFER_SIZE = 1024;
const int DEFAULT_PORT = 10000;
const int DEFAULT_MAX_USERS = 10;
const int NUMBER_OF_STREAMS = 3; //(stdin,stdout, stderr)
const string NAME_OF_LOGFILE = "logfile.log";

/** end of message */
const char END_OF_MESSAGE = '\n';
/** delimiter in incoming messages */
const char DELIMITER = '|';
/** 30 seconds */
const int WAITING_TIME_FOR_OPPONENT = 30;

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
