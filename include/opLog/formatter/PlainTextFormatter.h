#ifndef PLAINTEXTFORMATTER_H
#define PLAINTEXTFORMATTER_H

#include "FormatStyle.h"
#include "IFormatter.h"

class PlainTextFormatter final : public IFormatter {

private:
    static std::string logLevelToString(const LogLevel& logLevel);
    FormatStyle style{FormatStyle::STYLE_WITH_BRACKETS};
public:
    PlainTextFormatter() = default;
    explicit PlainTextFormatter(FormatStyle style);

    std::string format(const LogRecord& record) override;
};

#endif //PLAINTEXTFORMATTER_H
