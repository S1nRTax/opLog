#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include "opLog/Config.h"


namespace opLog {
    Config* Config::instance = nullptr;

    Config &Config::getInstance() {
        if (instance == nullptr) {
            instance = new Config();
            instance->setDefaultConfig();
        }
        return *instance;
    }

    void Config::setDefaultConfig() {
    // Defaults are already set in member initializers
    // This method can be used to reset to defaults if needed
}

std::string Config::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> Config::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

void Config::parseConfigFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filepath);
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        line = trim(line);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Parse key=value pairs
        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos) {
            std::cerr << "Warning: Invalid line " << lineNumber << " in config: " << line << std::endl;
            continue;
        }

        std::string key = trim(line.substr(0, equalPos));
        std::string value = trim(line.substr(equalPos + 1));

        // Remove quotes if present
        if ((value.front() == '"' && value.back() == '"') ||
            (value.front() == '\'' && value.back() == '\'')) {
            value = value.substr(1, value.length() - 2);
        }

        // Parse configuration values
        try {
            if (key == "log_directory") {
                logDirectory = value;
            } else if (key == "format_style") {
                if (value == "with_brackets" || value == "STYLE_WITH_BRACKETS") {
                    formatStyle = FormatStyle::STYLE_WITH_BRACKETS;
                } else if (value == "no_brackets" || value == "STYLE_NO_BRACKETS") {
                    formatStyle = FormatStyle::STYLE_NO_BRACKETS;
                } else {
                    std::cerr << "Warning: Unknown format style: " << value << std::endl;
                }
            } else if (key == "min_log_level") {
                if (value == "TRACE") minLogLevel = LogLevel::TRACE;
                else if (value == "DEBUG") minLogLevel = LogLevel::DEBUG;
                else if (value == "INFO") minLogLevel = LogLevel::INFO;
                else if (value == "WARN") minLogLevel = LogLevel::WARN;
                else if (value == "ERROR") minLogLevel = LogLevel::ERROR;
                else if (value == "FATAL") minLogLevel = LogLevel::FATAL;
                else std::cerr << "Warning: Unknown log level: " << value << std::endl;
            } else if (key == "max_file_size") {
                maxFileSize = std::stoull(value);
            } else if (key == "max_backup_files") {
                maxBackupFiles = std::stoi(value);
            } else if (key == "enable_colors") {
                enableColors = (value == "true" || value == "1" || value == "yes");
            } else if (key == "enable_timestamp") {
                enableTimestamp = (value == "true" || value == "1" || value == "yes");
            } else if (key == "datetime_format") {
                dateTimeFormat = value;
            } else if (key == "auto_flush") {
                autoFlush = (value == "true" || value == "1" || value == "yes");
            } else if (key == "trace_color") {
                colors.trace = value;
            } else if (key == "debug_color") {
                colors.debug = value;
            } else if (key == "info_color") {
                colors.info = value;
            } else if (key == "warn_color") {
                colors.warn = value;
            } else if (key == "error_color") {
                colors.error = value;
            } else if (key == "fatal_color") {
                colors.fatal = value;
            } else {
                std::cerr << "Warning: Unknown configuration key: " << key << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line " << lineNumber << ": " << e.what() << std::endl;
        }
    }

    file.close();
}

void Config::reloadConfig() {
    if (!configFilePath.empty()) {
        parseConfigFile(configFilePath);
    }
}

void Config::saveConfig(const std::string& filepath) {
    std::string outputPath = filepath.empty() ? configFilePath : filepath;
    if (outputPath.empty()) {
        outputPath = "oplog.conf";
    }

    std::ofstream file(outputPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot write config file: " + outputPath);
    }

    file << "# opLog Configuration File\n";
    file << "# Generated automatically\n\n";

    file << "# Log directory path\n";
    file << "log_directory=" << logDirectory << "\n\n";

    file << "# Format style: with_brackets or no_brackets\n";
    file << "format_style=" << (formatStyle == FormatStyle::STYLE_WITH_BRACKETS ? "with_brackets" : "no_brackets") << "\n\n";

    file << "# Minimum log level: TRACE, DEBUG, INFO, WARN, ERROR, FATAL\n";
    file << "min_log_level=";
    switch (minLogLevel) {
        case LogLevel::TRACE: file << "TRACE"; break;
        case LogLevel::DEBUG: file << "DEBUG"; break;
        case LogLevel::INFO: file << "INFO"; break;
        case LogLevel::WARN: file << "WARN"; break;
        case LogLevel::ERROR: file << "ERROR"; break;
        case LogLevel::FATAL: file << "FATAL"; break;
    }
    file << "\n\n";

    file << "# File rotation settings\n";
    file << "max_file_size=" << maxFileSize << "\n";
    file << "max_backup_files=" << maxBackupFiles << "\n\n";

    file << "# Display settings\n";
    file << "enable_colors=" << (enableColors ? "true" : "false") << "\n";
    file << "enable_timestamp=" << (enableTimestamp ? "true" : "false") << "\n";
    file << "datetime_format=" << dateTimeFormat << "\n";
    file << "auto_flush=" << (autoFlush ? "true" : "false") << "\n\n";

    file << "# Log level colors (ANSI escape sequences)\n";
    file << "trace_color=" << colors.trace << "\n";
    file << "debug_color=" << colors.debug << "\n";
    file << "info_color=" << colors.info << "\n";
    file << "warn_color=" << colors.warn << "\n";
    file << "error_color=" << colors.error << "\n";
    file << "fatal_color=" << colors.fatal << "\n";

    file.close();
    std::cout << "Config saved to: " << outputPath << std::endl;
}

void Config::printCurrentConfig() const {
    std::cout << "\n=== Current opLog Configuration ===" << std::endl;
    std::cout << "Log Directory: " << logDirectory << std::endl;
    std::cout << "Format Style: " << (formatStyle == FormatStyle::STYLE_WITH_BRACKETS ? "with_brackets" : "no_brackets") << std::endl;
    std::cout << "Min Log Level: ";
    switch (minLogLevel) {
        case LogLevel::TRACE: std::cout << "TRACE"; break;
        case LogLevel::DEBUG: std::cout << "DEBUG"; break;
        case LogLevel::INFO: std::cout << "INFO"; break;
        case LogLevel::WARN: std::cout << "WARN"; break;
        case LogLevel::ERROR: std::cout << "ERROR"; break;
        case LogLevel::FATAL: std::cout << "FATAL"; break;
    }
    std::cout << std::endl;
    std::cout << "Max File Size: " << maxFileSize << " bytes" << std::endl;
    std::cout << "Max Backup Files: " << maxBackupFiles << std::endl;
    std::cout << "Colors Enabled: " << (enableColors ? "Yes" : "No") << std::endl;
    std::cout << "Timestamp Enabled: " << (enableTimestamp ? "Yes" : "No") << std::endl;
    std::cout << "DateTime Format: " << dateTimeFormat << std::endl;
    std::cout << "Auto Flush: " << (autoFlush ? "Yes" : "No") << std::endl;
    std::cout << "==================================\n" << std::endl;
}


    void Config::initialize(const std::string &configPath) {
        Config& config = Config::getInstance();

        std::string actualConfigPath = configPath;
        if (actualConfigPath.empty()) {
            std::vector<std::string> possiblePaths = {
                "./opLog.conf",
                "./config/opLog.conf",
                "~/.config/opLog/opLog.conf",
                "etc/opLog/opLog.conf"
            };

            for (const auto& path : possiblePaths) {
                if (std::filesystem::exists(path)) {
                    actualConfigPath = path;
                    break;
                }
            }
        }

        if (!actualConfigPath.empty() && std::filesystem::exists(actualConfigPath)) {
            config.configFilePath = actualConfigPath;
            config.parseConfigFile(actualConfigPath);
            std::cout << "Loaded config from: " << actualConfigPath << std::endl;
        } else {
            std::cout << "No config file found, using defaults" << std::endl;
        }
    }

}
