#include "opLog/appender/FileAppender.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include "opLog/Config.h"

// FileAppender (Essentials):
// Writes to files - done
// Automatic files creation - done
// File rotation (by size/date/log-severity)
// Buffered modes (I'm not going to use buffered since buffered mode is slow asf)
// (Thread-safe file access)


FileAppender::FileAppender() : currentFileSize(0) {}


// [YYYY-mm-dd]-log.txt
std::string FileAppender::generateFileName(const std::string& message) {
    const auto& config = opLog::Config::getInstance();
    std::string filename;

    // Extract date from message for filename
    if (!message.empty() && message[0] == '[') {
        size_t closeBracket = message.find(']');
        if (closeBracket != std::string::npos) {
            std::string dateTime = message.substr(1, closeBracket - 1);
            // Extract just the date part (first 10 characters: YYYY-MM-DD)
            if (dateTime.length() >= 10) {
                filename = dateTime.substr(0, 10) + "-log.txt";
            } else {
                filename = "current-log.txt";
            }
        } else {
            filename = "current-log.txt";
        }
    } else {
        // No timestamp in message, use current date
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto* localTime = std::localtime(&time_t);

        std::ostringstream oss;
        oss << std::put_time(localTime, "%Y-%m-%d");
        filename = oss.str() + "-log.txt";
    }

    // Combine with log directory from config
    std::filesystem::path fullPath = std::filesystem::path(config.getLogDirectory()) / filename;
    return fullPath.string();
}

bool FileAppender::needsRotation(const std::string& filepath) {
    const auto& config = opLog::Config::getInstance();

    if (!std::filesystem::exists(filepath)) {
        return false;
    }

    auto fileSize = std::filesystem::file_size(filepath);
    return fileSize >= config.getMaxFileSize();
}


void FileAppender::rotateFile(const std::string& filepath) {
    const auto& config = opLog::Config::getInstance();

    if (!std::filesystem::exists(filepath)) {
        return;
    }

    std::cout << "Debug: Rotating file " << filepath << std::endl;

    // Parse components
    std::filesystem::path basePath(filepath);
    std::string baseName   = basePath.stem().string();
    std::string extension  = basePath.extension().string();
    std::string directory  = basePath.parent_path().string();

    // ---- Remove oldest backup ----
    {
        std::string oldestBackup;
        oldestBackup.reserve(directory.size() + baseName.size() + extension.size() + 16);
        oldestBackup.append(directory).append("/").append(baseName).append(".")
                    .append(std::to_string(config.getMaxBackupFiles()))
                    .append(extension);

        if (std::filesystem::exists(oldestBackup)) {
            std::filesystem::remove(oldestBackup);
        }
    }

    // ---- Shift backup files ----
    std::string currentBackup;
    std::string nextBackup;
    currentBackup.reserve(directory.size() + baseName.size() + extension.size() + 16);
    nextBackup.reserve(directory.size() + baseName.size() + extension.size() + 16);

    for (int i = config.getMaxBackupFiles() - 1; i >= 1; --i) {
        currentBackup.clear();
        currentBackup.append(directory).append("/").append(baseName).append(".")
                     .append(std::to_string(i)).append(extension);

        nextBackup.clear();
        nextBackup.append(directory).append("/").append(baseName).append(".")
                  .append(std::to_string(i + 1)).append(extension);

        if (std::filesystem::exists(currentBackup)) {
            std::filesystem::rename(currentBackup, nextBackup);
        }
    }

    // ---- Move current file to .1 ----
    std::string firstBackup;
    firstBackup.reserve(directory.size() + baseName.size() + extension.size() + 8);
    firstBackup.append(directory).append("/").append(baseName).append(".1").append(extension);

    std::filesystem::rename(filepath, firstBackup);

    currentFileSize = 0; // Reset file size counter
}


void FileAppender::write(const std::string& message) {
    try {
        const auto& config = opLog::Config::getInstance();
        const std::string filename = generateFileName(message);

        // Create logs directory if it doesn't exist
        std::filesystem::path filePath(filename);
        std::filesystem::path logDir = filePath.parent_path();

        if (!logDir.empty() && !std::filesystem::exists(logDir)) {
            std::cout << "Debug: Creating log directory: " << logDir << std::endl;
            std::filesystem::create_directories(logDir);
        }

        // Check if file rotation is needed
        if (needsRotation(filename)) {
            rotateFile(filename);
        }

        // Open file in append mode
        std::ofstream outFile(filename, std::ios::out | std::ios::app);

        if (!outFile.is_open()) {
            throw std::runtime_error("Error opening output file: " + filename);
        }

        // Write the message with a newline
        outFile << message << std::endl;

        // Update file size counter
        currentFileSize += message.length() + 1; // +1 for newline

        // Flush if auto-flush is enabled
        if (config.isAutoFlushEnabled()) {
            outFile.flush();
        }

        // Check if write was successful
        if (outFile.fail()) {
            throw std::runtime_error("Error writing to file: " + filename);
        }

        outFile.close();

        std::cout << "Debug: Successfully wrote to file: " << filename << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "FileAppender error: " << e.what() << std::endl;
        throw;
    }
}