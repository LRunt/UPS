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

// MESSAGES
const string MESSAGE_PING = "PING";
const string MESSAGE_ERROR = "ERROR";
