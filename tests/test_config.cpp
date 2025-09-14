#include "opLog/Config.h"
#include "opLog/appender/FileAppender.h"
#include "opLog/formatter/PlainTextFormatter.h"
#include <iostream>

int main() {
    try {
        // Method 1: Initialize with default config file search
        // This will look for oplog.conf in common locations
        opLog::Config::initialize();
        
        // Method 2: Initialize with specific config file
        // opLog::Config::initialize("./my-custom-config.conf");
        
        // Method 3: Programmatic configuration (overrides file config)
        auto& config = opLog::Config::getInstance();
        config.setLogDirectory("./custom-logs");
        config.setFormatStyle(FormatStyle::STYLE_NO_BRACKETS);
        config.setMinLogLevel(LogLevel::DEBUG);
        config.setColorsEnabled(true);
        
        // Print current configuration
        config.printCurrentConfig();
        
        // Save current configuration to file
        config.saveConfig("./generated-oplog.conf");
        
        // Test the configured logging
        FileAppender appender;
        PlainTextFormatter formatter;
        
        // Create some test log records
        LogRecord debugRecord;
        debugRecord.logLevel = LogLevel::DEBUG;
        debugRecord.message = "Debug message for testing";
        debugRecord.timestamp = std::chrono::system_clock::now();
        
        LogRecord infoRecord;
        infoRecord.logLevel = LogLevel::INFO;
        infoRecord.message = "Information message";
        infoRecord.timestamp = std::chrono::system_clock::now();
        
        LogRecord errorRecord;
        errorRecord.logLevel = LogLevel::ERROR;
        errorRecord.message = "Error message that should be logged";
        errorRecord.timestamp = std::chrono::system_clock::now();

        // Format and write the messages
        std::string formattedDebug = formatter.format(debugRecord);
        std::string formattedInfo = formatter.format(infoRecord);
        std::string formattedError = formatter.format(errorRecord);

        if (!formattedDebug.empty()) {
            std::cout << "Console output: " << formattedDebug << std::endl;
            appender.write(formattedDebug);
        }

        if (!formattedInfo.empty()) {
            std::cout << "Console output: " << formattedInfo << std::endl;
            appender.write(formattedInfo);
        }

        if (!formattedError.empty()) {
            std::cout << "Console output: " << formattedError << std::endl;
            appender.write(formattedError);
        }

        std::cout << "\n=== Testing Configuration Changes ===" << std::endl;

        // Test runtime configuration changes
        std::cout << "Changing to bracket format..." << std::endl;
        config.setFormatStyle(FormatStyle::STYLE_WITH_BRACKETS);

        std::cout << "Disabling colors..." << std::endl;
        config.setColorsEnabled(false);

        std::cout << "Setting minimum level to ERROR..." << std::endl;
        config.setMinLogLevel(LogLevel::ERROR);

        // Test with new configuration
        PlainTextFormatter newFormatter;

        LogRecord testRecord;
        testRecord.logLevel = LogLevel::WARN;
        testRecord.message = "Warning that should be filtered out";
        testRecord.timestamp = std::chrono::system_clock::now();

        LogRecord testRecord2;
        testRecord2.logLevel = LogLevel::FATAL;
        testRecord2.message = "Fatal error that should appear";
        testRecord2.timestamp = std::chrono::system_clock::now();

        std::string formattedWarn = newFormatter.format(testRecord);
        std::string formattedFatal = newFormatter.format(testRecord2);

        if (formattedWarn.empty()) {
            std::cout << "WARN message correctly filtered out" << std::endl;
        } else {
            std::cout << "WARN: " << formattedWarn << std::endl;
        }

        if (!formattedFatal.empty()) {
            std::cout << "FATAL: " << formattedFatal << std::endl;
            appender.write(formattedFatal);
        }

        // Reload configuration from file
        std::cout << "\n=== Reloading Configuration ===" << std::endl;
        config.reloadConfig();
        config.printCurrentConfig();

        std::cout << "\nConfiguration test completed successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// Example of creating a custom configuration programmatically
void createCustomConfig() {
    auto& config = opLog::Config::getInstance();

    // Set up a custom configuration for a production environment
    config.setLogDirectory("/var/log/myapp");
    config.setFormatStyle(FormatStyle::STYLE_NO_BRACKETS);
    config.setMinLogLevel(LogLevel::INFO);
    config.setMaxFileSize(50 * 1024 * 1024); // 50MB
    config.setMaxBackupFiles(10);
    config.setColorsEnabled(false); // No colors in production logs
    config.setAutoFlushEnabled(true); // Ensure logs are written immediately
    config.setDateTimeFormat("%Y-%m-%d %H:%M:%S.%f"); // Include microseconds

    // Custom colors for development environment
    config.setTraceColor("\033[90m");     // Dark gray
    config.setDebugColor("\033[36m");     // Cyan
    config.setInfoColor("\033[32m");      // Green
    config.setWarnColor("\033[93m");      // Bright yellow
    config.setErrorColor("\033[91m");     // Bright red
    config.setFatalColor("\033[95m");     // Bright magenta

    // Save this configuration
    config.saveConfig("production-oplog.conf");
}