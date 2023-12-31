#include "Logger.h"

Logger& Logger::instance() {
    static Logger instance;  // Singleton pattern to have a single logger instance
    return instance;
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    logFile_.open(filename, std::ios::out | std::ios::app);
    if (!logFile_.is_open()) {
        std::cerr << "Error opening log file: " << filename << std::endl;
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Get current timestamp
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    std::ostringstream timestamp;
    timestamp << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");

    // Log to console
    std::cout << "[" << timestamp.str() << "] [" << logLevelToString(level) << "] " << message << std::endl;

    // Log to file if it's open
    if (logFile_.is_open()) {
        logFile_ << "[" << timestamp.str() << "] [" << logLevelToString(level) << "] " << message << std::endl;
        logFile_.flush();
    }
}

std::string Logger::logLevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
    }
    return "";
}