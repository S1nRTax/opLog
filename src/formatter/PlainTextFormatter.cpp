#include "opLog/formatter/PlainTextFormatter.h"
#include <iomanip>
#include <sstream>

PlainTextFormatter::PlainTextFormatter(const FormatStyle style) : style(style) {}

std::string PlainTextFormatter::logLevelToString(const LogLevel& logLevel) {
    static const char* LOG_LEVEL_STRINGS[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
    };
    return LOG_LEVEL_STRINGS[static_cast<size_t>(logLevel)];
}

std::string PlainTextFormatter::format(const LogRecord& record) {
    const std::time_t time = std::chrono::system_clock::to_time_t(record.timestamp);
    const std::tm* localTime = std::localtime(&time);

    std::ostringstream oss;

    switch (style) {
        case FormatStyle::STYLE_WITH_BRACKETS:
            // [YYYY-MM-DD] [log severity] [message]
            oss << "[" <<std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << "] "
            << "[" << logLevelToString(record.logLevel) << "] "
            << record.message ; break;

        case FormatStyle::STYLE_NO_BRACKETS:
            // YYYY-MM-DD log-severity message
            oss <<std::put_time(localTime, "%Y-%m-%d %H:%M:%S")
            << " " << logLevelToString(record.logLevel) << " "
            << record.message ; break;
    }

    return oss.str();
}
