#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include "formatter/IFormatter.h"
#include "appender/IAppender.h"
#include "LogLevel.h"


class Logger {

private:
    std::unique_ptr<IFormatter> formatter_;
    std::unique_ptr<IAppender> appenders_;
    LogLevel minLevel_;

public:
    Logger(std::unique_ptr<IFormatter> aFormatter,
        std::unique_ptr<IAppender> aAppender,
        const LogLevel& logLevel
        );

    void log(LogLevel level, const std::string& message) const;

    void debug(const std::string& message) const;
    void info(const std::string& message) const;
    void error(const std::string& message) const;
    void warn(const std::string& message) const;
    void fatal(const std::string& message) const;
    void trace(const std::string& message) const;

};

#endif //LOGGER_H
