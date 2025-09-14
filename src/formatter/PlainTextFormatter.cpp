#include "opLog/formatter/PlainTextFormatter.h"
#include "opLog/Config.h"
#include<array>
#include <iomanip>
#include <sstream>

PlainTextFormatter::PlainTextFormatter(const FormatStyle style) : style(style) {}

std::string PlainTextFormatter::logLevelToString(const LogLevel& logLevel) {
    const auto idx = static_cast<size_t>(logLevel);
    const auto& config = opLog::Config::getInstance();
    const auto& colors = config.getColors();

    static constexpr const char* LOG_LEVEL_STRINGS[]{
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
    };

    const std::array<std::reference_wrapper<const std::string>, 6> COLOR_LOOKUP = {
        colors.trace, colors.debug, colors.info, colors.warn, colors.error, colors.fatal
    };

    if (config.isColorsEnabled()) {
        return COLOR_LOOKUP[idx].get() + LOG_LEVEL_STRINGS[idx] + colors.reset;
    }
        return LOG_LEVEL_STRINGS[idx];
}

std::string PlainTextFormatter::format(const LogRecord& record) {
    const auto& config = opLog::Config::getInstance();

    // Check if this log level should be processed
    if (record.logLevel < config.getMinLogLevel()) {
        return ""; // Skip this message
    }

    std::ostringstream oss;

    // Add timestamp if enabled
    if (config.isTimestampEnabled()) {
        const std::time_t time = std::chrono::system_clock::to_time_t(record.timestamp);
        const std::tm* localTime = std::localtime(&time);

        // Use format style from config (or override with constructor parameter)
        FormatStyle actualStyle = (style != FormatStyle::STYLE_WITH_BRACKETS &&
                                  style != FormatStyle::STYLE_NO_BRACKETS) ?
                                 config.getFormatStyle() : style;

        switch (actualStyle) {
            case FormatStyle::STYLE_WITH_BRACKETS:
                // [YYYY-MM-DD HH:MM:SS] [LOG_LEVEL] message
                oss << "[";
                oss << std::put_time(localTime, config.getDateTimeFormat().c_str());
                oss << "] [" << logLevelToString(record.logLevel) << "] "
                    << record.message;
                break;

            case FormatStyle::STYLE_NO_BRACKETS:
                // YYYY-MM-DD HH:MM:SS LOG_LEVEL message
                oss << std::put_time(localTime, config.getDateTimeFormat().c_str()) << " "
                    << logLevelToString(record.logLevel) << " "
                    << record.message;
                break;
        }
    } else {
        // No timestamp, just level and message
        oss << "[" << logLevelToString(record.logLevel) << "] " << record.message;
    }

    return oss.str();
}
