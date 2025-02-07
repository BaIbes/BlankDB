// Logger.cpp
#include "../../include/blankdb/utils/Logger.hpp"
#include <sstream>
#include <iomanip>

namespace blankdb {
// Define and initialize the static instance pointer
std::unique_ptr<Logger> Logger::instance_;

Logger& Logger::get_instance() {
    if (!instance_) {
        // Initialize logger with a default log file path
        instance_.reset(new Logger("blankdb.log"));
    }
    return *instance_;
}

Logger::Logger(const std::string& log_file_path)
    : log_file_path_(log_file_path), log_file_(log_file_path, std::ios::app) {}

Logger::~Logger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

// Convenience methods
void Logger::info(const std::string& message) {
    log(message, Level::INFO);
}

void Logger::error(const std::string& message) {
    log(message, Level::ERROR);
}

void Logger::debug(const std::string& message) {
    log(message, Level::DEBUG);
}

// Generic logging method
void Logger::log(const std::string& message, Level level) {
    std::lock_guard<std::mutex> lock(log_mutex_); // Ensure thread safety

    // Format the log message
    std::string formatted_message = format_message(message, level);

    // Write to console
    std::cout << formatted_message << std::endl;

    // Write to file
    if (log_file_.is_open()) {
        log_file_ << formatted_message << std::endl;
        log_file_.flush(); // Ensure data is written immediately
    }
}

// Helper method to format log messages
std::string Logger::format_message(const std::string& message, Level level) const {
    // Get current timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_time;
    localtime_s(&local_time, &time_t); // Use localtime_s for thread safety

    // Format timestamp
    std::ostringstream oss;
    oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    // Determine log level string
    std::string level_str;
    switch (level) {
        case Level::INFO:  level_str = "INFO"; break;
        case Level::ERROR: level_str = "ERROR"; break;
        case Level::DEBUG: level_str = "DEBUG"; break;
    }

    // Combine all parts into the final log message
    return "[" + oss.str() + "] [" + level_str + "] " + message;
}
} // namespace blankdb