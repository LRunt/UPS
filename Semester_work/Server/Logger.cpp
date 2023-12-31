// Logger.cpp
#include "Logger.h"

Logger& Logger::instance() {
    static Logger instance; // Meyers' singleton pattern
    return instance;
}

template <typename T>
void Logger::log(const T& message) {
    std::cout << getCurrentTime() << " " << message << std::endl; // Log to console with timestamp
    if (fileStream.is_open()) {
        fileStream << getCurrentTime() << " " << message << std::endl; // Log to file with timestamp
    }
}

template <typename T>
void Logger::error(const T& message) {
    std::cerr << getCurrentTime() << " [ERROR] " << message << std::endl; // Log error to console with timestamp
    if (fileStream.is_open()) {
        fileStream << getCurrentTime() << " [ERROR] " << message << std::endl; // Log error to file with timestamp
    }
}

void Logger::setLogFile(const std::string& filename) {
    fileStream.close(); // Close any existing file stream
    fileStream.open(filename, std::ios::app); // Open or create the log file in append mode
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    return std::ctime(&now_time);
}