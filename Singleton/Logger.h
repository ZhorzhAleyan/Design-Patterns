#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <mutex>

class Logger {
public:
    enum class Level { INFO, WARNING, ERROR };

    // Global access point - returns the one and only instance.
    static Logger& instance() {
        static Logger instance; // created once, thread-safe since C++11
        return instance;
    }

    // Not copyable and not movable - there can be only one instance.
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(Level level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        ++messageCount_;
        std::cout << "[" << levelToString(level) << "] "
                  << "(#" << messageCount_ << ") "
                  << message << std::endl;
    }

    int messageCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return messageCount_;
    }

private:
    Logger() = default; // private constructor - only instance() can create it

    static std::string levelToString(Level level) {
        switch (level) {
            case Level::INFO:    return "INFO";
            case Level::WARNING: return "WARNING";
            case Level::ERROR:   return "ERROR";
        }
        return "UNKNOWN";
    }

    mutable std::mutex mutex_;
    int messageCount_ = 0;
};

#endif // LOGGER_H
