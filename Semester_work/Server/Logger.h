// Logger.h
#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

class Logger {
public:
    static Logger& instance();

    template <typename T>
    void log(const T& message);

    template <typename T>
    void error(const T& message);

    void setLogFile(const std::string& filename);

private:
    Logger(); // Private constructor for singleton pattern
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream fileStream; // File output stream

    std::string getCurrentTime();
};
