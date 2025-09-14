#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <vector>
#include <mutex>
#include "formatter/IFormatter.h"
#include "appender/IAppender.h"
#include "LogLevel.h"

class Logger {
private:
    std::unique_ptr<IFormatter> formatter_;
    std::vector<std::unique_ptr<IAppender>> appenders_;
    mutable std::mutex logMutex_; // For thread safety

    // Static instance for singleton pattern
    static std::unique_ptr<Logger> instance_;
    static std::once_flag instanceFlag_;

public:
    // Constructor for custom logger
    Logger(std::unique_ptr<IFormatter> formatter = nullptr,
           std::vector<std::unique_ptr<IAppender>> appenders = {});

    // Explicitly delete copy and move constructors/assignment operators
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    // Static factory methods
    static Logger& getInstance();
    static Logger createFileLogger(const std::string& configPath = "");
    static Logger createConsoleLogger();
    static Logger createDualLogger(); // Both file and console

    // Core logging method
    void log(LogLevel level, const std::string& message) const;

    // Convenience methods
    void trace(const std::string& message) const;
    void debug(const std::string& message) const;
    void info(const std::string& message) const;
    void warn(const std::string& message) const;
    void error(const std::string& message) const;
    void fatal(const std::string& message) const;

    // Formatted logging (printf-style)
    template<typename... Args>
    void logf(LogLevel level, const std::string& format, Args&&... args) const;

    template<typename... Args>
    void infof(const std::string& format, Args&&... args) const;

    template<typename... Args>
    void errorf(const std::string& format, Args&&... args) const;

    template<typename... Args>
    void debugf(const std::string& format, Args&&... args) const;

    // Appender management
    void addAppender(std::unique_ptr<IAppender> appender);
    void clearAppenders();
    size_t getAppenderCount() const;

    // Configuration
    void setFormatter(std::unique_ptr<IFormatter> formatter);
    void reloadConfig();

    // Utility methods
    bool shouldLog(LogLevel level) const;
    void flush() const; // Force flush all appenders
};

// Template implementations
#include <sstream>
#include <iomanip>

template<typename... Args>
void Logger::logf(LogLevel level, const std::string& format, Args&&... args) const {
    if (!shouldLog(level)) return;

    // Simple sprintf-style formatting
    char buffer[1024];
    std::snprintf(buffer, sizeof(buffer), format.c_str(), args...);
    log(level, std::string(buffer));
}

template<typename... Args>
void Logger::infof(const std::string& format, Args&&... args) const {
    logf(LogLevel::INFO, format, std::forward<Args>(args)...);
}

template<typename... Args>
void Logger::errorf(const std::string& format, Args&&... args) const {
    logf(LogLevel::ERROR, format, std::forward<Args>(args)...);
}

template<typename... Args>
void Logger::debugf(const std::string& format, Args&&... args) const {
    logf(LogLevel::DEBUG, format, std::forward<Args>(args)...);
}

#endif // LOGGER_H