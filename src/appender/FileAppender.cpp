#include "opLog/appender/FileAppender.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

// FileAppender (Essentials):
// Writes to files - done
// Automatic files creation - done
// File rotation (by size/date/log-severity)
// Buffered modes (I'm not going to use buffered since buffered mode is slow asf)
// (Thread-safe file access)

// [YYYY-mm-dd]-log.txt
std::string generateUniqueFileName(const std::string& message) {
    std::string filename;

    // Case 1: message with brackets [YYYY-mm-dd]
    if (!message.empty() && message[0] == '[') {
        size_t closeBracket = message.find(']');
        if (closeBracket != std::string::npos) {
            filename = message.substr(1, closeBracket - 1) + "-log.txt";
        } else {
            // Malformed bracket, fallback
            filename = "malformed-log.txt";
        }
    }
    // Case 2: message without brackets - use first word
    else if (!message.empty()) {
        size_t spacePos = message.find_first_of(' ');
        if (spacePos != std::string::npos) {
            filename = message.substr(0, spacePos) + "-log.txt";
        } else {
            // No space found, use entire message (truncated if too long)
            const std::string truncated = message.length() > 20 ? message.substr(0, 20) : message;
            filename = truncated + "-log.txt";
        }
    }
    // Case 3: empty message
    else {
        filename = "empty-log.txt";
    }

    std::cout << "Debug: Generated filename: '" << filename << "'" << std::endl;
    return filename;
}


void FileAppender::write(const std::string& message) {
    try {
        const std::string filename = generateUniqueFileName(message);

        // Check if file exists before writing
        const bool fileExistedBefore = std::filesystem::exists(filename);
        std::cout << "Debug: File existed before write: " << (fileExistedBefore ? "YES" : "NO") << std::endl;

        // append mode
        std::ofstream outFile(filename, std::ios::out | std::ios::app);

        if (!outFile.is_open()) {
            throw std::runtime_error("Error opening output file: " + filename);
        }

        outFile << message << std::endl;
        // Flush to ensure data is written
        outFile.flush();

        if (outFile.fail()) {
            throw std::runtime_error("Error writing to file: " + filename);
        }

        outFile.close();
        // Check if file exists after writing
        const bool fileExistsAfter = std::filesystem::exists(filename);
        std::cout << "Debug: File exists after write: " << (fileExistsAfter ? "YES" : "NO") << std::endl;

        if (fileExistsAfter) {
            const auto fileSize = std::filesystem::file_size(filename);
            std::cout << "Debug: File size: " << fileSize << " bytes" << std::endl;
        }

        std::cout << "Debug: Successfully wrote to file: " << filename << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "FileAppender error: " << e.what() << std::endl;
        throw; // Re-throw the exception
    }
}