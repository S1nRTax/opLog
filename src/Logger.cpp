#include "opLog/Logger.h"
#include "opLog/LogRecord.h"
#include <chrono>


Logger::Logger(std::unique_ptr<IFormatter> aFormatter,
     std::unique_ptr<IAppender> aAppender,
     const LogLevel& logLevel
     ): formatter_(std::move(aFormatter)),
        appenders_(std::move(aAppender)),
        minLevel_(logLevel){}



void Logger::log(const LogLevel level, const std::string& message) const {
        if (level < minLevel_)
            return; // filter out low-level logs.

        const LogRecord record{level, message, std::chrono::system_clock::now()};
        const std::string formatted{ formatter_->format(record) };

        appenders_->write(formatted);

}

void Logger::info(const std::string& message) const { log(LogLevel::INFO, message); }
void Logger::error(const std::string& message) const { log(LogLevel::ERROR, message); }
void Logger::debug(const std::string& message) const { log(LogLevel::DEBUG, message); }
void Logger::warn(const std::string& message) const { log(LogLevel::WARN, message); }
void Logger::fatal(const std::string& message) const { log(LogLevel::FATAL, message); }
void Logger::trace(const std::string& message) const { log(LogLevel::TRACE, message); }

