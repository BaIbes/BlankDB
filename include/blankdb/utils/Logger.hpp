#ifndef BLANKDB_LOGGER_HPP
#define BLANKDB_LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>
#include <chrono>
#include <ctime>

namespace blankdb {

class Logger {
public:
    // Log levels
    enum class Level {
        INFO,
        ERROR,
        DEBUG
    };

    // Singleton accessor
    static Logger& get_instance();

    void info(const std::string& message);
    void error(const std::string& message);
    void debug(const std::string& message);

    // Logging methods
    void log(const std::string& message, Level level = Level::INFO);

private:
    Logger(const std::string& log_file_path); // Private constructor for singleton
    ~Logger();                                // Private destructor for singleton

    // Disable copy and move operations
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Helper method to format log messages
    std::string format_message(const std::string& message, Level level) const;

    // Member variables
    static std::unique_ptr<Logger> instance_; // Static pointer to the singleton instance

    // Allow std::unique_ptr to call the private destructor
    friend std::default_delete<Logger>;

    std::string log_file_path_;
    std::ofstream log_file_;
    std::mutex log_mutex_; // Ensures thread-safe logging
};

} // namespace blankdb

#endif // BLANKDB_LOGGER_HPP