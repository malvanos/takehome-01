#pragma once
#include <string>

class Logger {
public:
    enum class LogLevel {
        INFO,
        WARNING,
        LOGERROR
    };

    virtual ~Logger() = default;

    virtual void log(LogLevel level, const std::string& message) = 0;

    virtual void info(const std::string& message) { log(LogLevel::INFO, message); }
    virtual void warning(const std::string& message) { log(LogLevel::WARNING, message); }
    virtual void error(const std::string& message) { log(LogLevel::LOGERROR, message); }

};