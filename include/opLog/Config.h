#ifndef CONFIG_H
#define CONFIG_H


#include <string>
#include <unordered_map>
#include <vector>
#include "opLog/formatter/FormatStyle.h"
#include "opLog/LogLevel.h"

namespace opLog {

    struct LogLevelColors {
        std::string trace = "\033[36m";      // Cyan
        std::string debug = "\033[34m";      // Blue
        std::string info = "\033[32m";       // Green
        std::string warn = "\033[33m";       // Yellow
        std::string error = "\033[31m";      // Red
        std::string fatal = "\033[1;31m";    // Bold Red
        std::string reset = "\033[0m";       // Reset
    };


    class Config {
        private:
            static Config* instance;
            std::string configFilePath;

            // Default config values
            std::string logDirectory{"./logs"};
            FormatStyle formatStyle{FormatStyle::STYLE_WITH_BRACKETS};
            LogLevelColors colors;
            LogLevel minLogLevel{LogLevel::TRACE};
            size_t maxFileSize{10 * 1024 * 1024}; // 10mb
            int maxBackupFiles{5};
            bool enableColors{true};
            bool enableTimestamp{true};
            std::string dateTimeFormat = "%Y-%m-%d %H:%M:%S";
            bool autoFlush = true;

            Config() = default;

            void parseConfigFile(const std::string& filepath);
            void setDefaultConfig();
            std::string trim(const std::string& str);
            std::vector<std::string> split(const std::string& str, char delimiter);

    public:
        static Config& getInstance();
        static void initialize(const std::string& configPath = "");

        //Getters:
        const std::string& getLogDirectory() const { return logDirectory; }
        FormatStyle getFormatStyle() const { return formatStyle; }
        const LogLevelColors& getColors() const { return colors; }
        LogLevel getMinLogLevel() const { return minLogLevel; }
        size_t getMaxFileSize() const { return maxFileSize; }
        int getMaxBackupFiles() const { return maxBackupFiles; }
        bool isColorsEnabled() const { return enableColors; }
        bool isTimestampEnabled() const { return enableTimestamp; }
        const std::string& getDateTimeFormat() const { return dateTimeFormat; }
        bool isAutoFlushEnabled() const { return autoFlush; }

        //Setters:
        void setLogDirectory(const std::string& dir) { logDirectory = dir; }
        void setFormatStyle(FormatStyle style) { formatStyle = style; }
        void setMinLogLevel(LogLevel level) { minLogLevel = level; }
        void setMaxFileSize(size_t size) { maxFileSize = size; }
        void setMaxBackupFiles(int count) { maxBackupFiles = count; }
        void setColorsEnabled(bool enabled) { enableColors = enabled; }
        void setTimestampEnabled(bool enabled) { enableTimestamp = enabled; }
        void setDateTimeFormat(const std::string& format) { dateTimeFormat = format; }
        void setAutoFlushEnabled(bool enabled) { autoFlush = enabled; }

        // Color setters
        void setTraceColor(const std::string& color) { colors.trace = color; }
        void setDebugColor(const std::string& color) { colors.debug = color; }
        void setInfoColor(const std::string& color) { colors.info = color; }
        void setWarnColor(const std::string& color) { colors.warn = color; }
        void setErrorColor(const std::string& color) { colors.error = color; }
        void setFatalColor(const std::string& color) { colors.fatal = color; }

        // Utility methods
        void reloadConfig();
        void saveConfig(const std::string& filepath = "");
        void printCurrentConfig() const;
    };



}

#endif //CONFIG_H
