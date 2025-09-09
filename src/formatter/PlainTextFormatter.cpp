#include "opLog/formatter/PlainTextFormatter.h"
#include <iomanip>
#include <sstream>


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
    oss << "[" <<std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << logLevelToString(record.logLevel) << "] "
        << record.message ;

    return oss.str();
}
