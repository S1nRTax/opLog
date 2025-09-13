#include "opLog/formatter/PlainTextFormatter.h"
#include <iomanip>
#include <sstream>

PlainTextFormatter::PlainTextFormatter(const FormatStyle style) : style(style) {}

std::string PlainTextFormatter::logLevelToString(const LogLevel& logLevel) {
    const auto idx = static_cast<size_t>(logLevel);

    static const char* LOG_LEVEL_STRINGS[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
    };

    static constexpr const char* LOG_LEVEL_COLORS[] = {
        "\033[36m", // TRACE = Cyan
        "\033[34m", // DEBUG = Blue
        "\033[32m", // INFO  = Green
        "\033[33m", // WARN  = Yellow
        "\033[31m", // ERROR = Red
        "\033[1;31m" // FATAL = Bold Red
    };

    return std::string(LOG_LEVEL_COLORS[idx]) + LOG_LEVEL_STRINGS[idx] + "\033[0m";
}

std::string PlainTextFormatter::format(const LogRecord& record) {
    const std::time_t time = std::chrono::system_clock::to_time_t(record.timestamp);
    const std::tm* localTime = std::localtime(&time);

    std::ostringstream oss;

    switch (style) {
        case FormatStyle::STYLE_WITH_BRACKETS:
            // [YYYY-MM-DD] [log severity] [message]
            oss << "[" << logLevelToString(record.logLevel) << "] "
                   "[" <<std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << "] "
                    << record.message ; break;

        case FormatStyle::STYLE_NO_BRACKETS:
            // YYYY-MM-DD log-severity message
            oss << " " << logLevelToString(record.logLevel) << " "
                << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << " "
                << record.message ; break;
    }

    return oss.str();
}
