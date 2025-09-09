#ifndef PLAINTEXTFORMATTER_H
#define PLAINTEXTFORMATTER_H

#include "IFormatter.h"

class PlainTextFormatter final : public IFormatter {

private:
    static std::string logLevelToString(const LogLevel& logLevel);
public:
    PlainTextFormatter() = default;
    std::string format(const LogRecord& record) override;
};

#endif //PLAINTEXTFORMATTER_H
