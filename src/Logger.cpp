#include "opLog/Logger.h"
#include "opLog/Config.h"
#include "opLog/LogRecord.h"
#include "opLog/formatter/PlainTextFormatter.h"
#include "opLog/appender/FileAppender.h"
#include "opLog/appender/ConsoleAppender.h"
#include <chrono>
#include <iostream>
#include <memory>

// Static members
std::unique_ptr<Logger> Logger::instance_ = nullptr;
std::once_flag Logger::instanceFlag_;

Logger::Logger(std::unique_ptr<IFormatter> formatter,
               std::vector<std::unique_ptr<IAppender>> appenders)
    : formatter_(std::move(formatter)), appenders_(std::move(appenders)) {

    // Set default formatter if none provided
    if (!formatter_) {
        formatter_ = std::make_unique<PlainTextFormatter>();
    }

    // Add default console appender if no appenders provided
    if (appenders_.empty()) {
        appenders_.push_back(std::make_unique<ConsoleAppender>());
    }
}

Logger& Logger::getInstance() {
    std::call_once(instanceFlag_, []() {
        // Initialize config
        opLog::Config::initialize();

        auto formatter = std::make_unique<PlainTextFormatter>();
        std::vector<std::unique_ptr<IAppender>> appenders;
        appenders.push_back(std::make_unique<FileAppender>());

        // Create Logger directly instead of using createFileLogger()
        instance_ = std::make_unique<Logger>(std::move(formatter), std::move(appenders));
    });
    return *instance_;
}

Logger Logger::createFileLogger(const std::string& configPath) {
    // Initialize config if not already done
    if (configPath.empty()) {
        opLog::Config::initialize();
    } else {
        opLog::Config::initialize(configPath);
    }

    auto formatter = std::make_unique<PlainTextFormatter>();

    std::vector<std::unique_ptr<IAppender>> appenders;
    appenders.push_back(std::make_unique<FileAppender>());

    return {std::move(formatter), std::move(appenders)};
}

Logger Logger::createConsoleLogger() {
    auto formatter = std::make_unique<PlainTextFormatter>();

    std::vector<std::unique_ptr<IAppender>> appenders;
    appenders.push_back(std::make_unique<ConsoleAppender>());

    return {std::move(formatter), std::move(appenders)};
}

Logger Logger::createDualLogger() {
    // Initialize config
    opLog::Config::initialize();

    auto formatter = std::make_unique<PlainTextFormatter>();

    std::vector<std::unique_ptr<IAppender>> appenders;
    appenders.push_back(std::make_unique<FileAppender>());
    appenders.push_back(std::make_unique<ConsoleAppender>());

    return {std::move(formatter), std::move(appenders)};
}

bool Logger::shouldLog(LogLevel level) const {
    const auto& config = opLog::Config::getInstance();
    return level >= config.getMinLogLevel();
}

void Logger::log(LogLevel level, const std::string& message) const {
    if (!shouldLog(level)) {
        return; // Filter out based on config
    }

    // Thread-safe logging
    std::lock_guard<std::mutex> lock(logMutex_);

    // Create log record
    const LogRecord record{level, message, std::chrono::system_clock::now()};

    // Format the message
    const std::string formatted = formatter_->format(record);

    // Skip empty formatted messages (filtered by formatter)
    if (formatted.empty()) {
        return;
    }

    // Write to all appenders
    for (const auto& appender : appenders_) {
        try {
            appender->write(formatted);
        } catch (const std::exception& e) {
            // Log to stderr if appender fails (avoid infinite recursion)
            std::cerr << "Logger: Appender error: " << e.what() << std::endl;
        }
    }
}

void Logger::trace(const std::string& message) const { log(LogLevel::TRACE, message); }
void Logger::debug(const std::string& message) const { log(LogLevel::DEBUG, message); }
void Logger::info(const std::string& message) const { log(LogLevel::INFO, message); }
void Logger::warn(const std::string& message) const { log(LogLevel::WARN, message); }
void Logger::error(const std::string& message) const { log(LogLevel::ERROR, message); }
void Logger::fatal(const std::string& message) const { log(LogLevel::FATAL, message); }

void Logger::addAppender(std::unique_ptr<IAppender> appender) {
    std::lock_guard<std::mutex> lock(logMutex_);
    appenders_.push_back(std::move(appender));
}

void Logger::clearAppenders() {
    std::lock_guard<std::mutex> lock(logMutex_);
    appenders_.clear();
}

size_t Logger::getAppenderCount() const {
    std::lock_guard<std::mutex> lock(logMutex_);
    return appenders_.size();
}

void Logger::setFormatter(std::unique_ptr<IFormatter> formatter) {
    std::lock_guard<std::mutex> lock(logMutex_);
    formatter_ = std::move(formatter);
}

void Logger::reloadConfig() {
    auto& config = opLog::Config::getInstance();
    config.reloadConfig();

    // TODO: recreate formatters/appenders
    // For example, update formatter style based on new config
    if (formatter_) {
        formatter_ = std::make_unique<PlainTextFormatter>();
    }
}

void Logger::flush() const {
    // TODO: implement IFlushable interface if appenders support flushing.
    std::lock_guard<std::mutex> lock(logMutex_);
    // Future: call flush() on each appender that supports it
}