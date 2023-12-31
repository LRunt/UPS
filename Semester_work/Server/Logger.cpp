#include "Logger.h"

Logger::Logger() {
    // Constructor implementation, if any
}

Logger& Logger::instance() {
    static Logger instance;  // Singleton instance
    return instance;
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    logFile_.open(filename, std::ios_base::app);
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Log to console
    std::cout << "[" << logLevelToString(level) << "] " << message << std::endl;

    // Log to file
    if (logFile_.is_open()) {
        logFile_ << "[" << logLevelToString(level) << "] " << message << std::endl;
    }
}

std::string Logger::logLevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}
