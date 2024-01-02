/**
 * Class Logger represents logger of the application
 *
 * @author Lukas Runt
 * @date 02-01-2024
 * @version 1.0.0
 */

#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <mutex>

enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger {
public:
    static Logger& instance();

    void setLogFile(const std::string& filename);

    void log(LogLevel level, const std::string& message);

private:
    Logger();  // Private constructor for singleton pattern

    std::string logLevelToString(LogLevel level) const;

    std::ofstream logFile_;
    std::mutex mutex_;
};

// Macros for easy logging
#define LOG_DEBUG(message) Logger::instance().log(LogLevel::DEBUG, message)
#define LOG_INFO(message) Logger::instance().log(LogLevel::INFO, message)
#define LOG_WARNING(message) Logger::instance().log(LogLevel::WARNING, message)
#define LOG_ERROR(message) Logger::instance().log(LogLevel::ERROR, message)

