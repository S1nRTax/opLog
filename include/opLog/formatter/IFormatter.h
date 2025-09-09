#ifndef FORMATTER_H
#define FORMATTER_H

#include <opLog/LogLevel.h>
#include <chrono>

struct LogRecord {
    LogLevel logLevel;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
};


class IFormatter {
public:
    virtual ~IFormatter() = default;
    virtual std::string format(const LogRecord& record) = 0;
};
#endif //FORMATTER_H
