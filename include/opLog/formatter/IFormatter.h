#ifndef FORMATTER_H
#define FORMATTER_H

#include <opLog/LogLevel.h>
#include <opLog/LogRecord.h>

class IFormatter {
public:
    virtual ~IFormatter() = default;
    virtual std::string format(const LogRecord& record) = 0;
};
#endif //FORMATTER_H
