#include<iostream>
#include "opLog/appender/ConsoleAppender.h"
#include "opLog/appender/FileAppender.h"

void unitConsoleAppender() {
    ConsoleAppender appender;
    appender.write("[2025-09-13 20:11:36] [WARN] this is a debug message");
    appender.write("2025-09-13 20:11:36 WARN this is a debug message");
}

void unitFileAppender() {
    try {
        FileAppender appender;
        appender.write("[2024-01-15] Test message 1");
        appender.write("SimpleMessage Test message 2");
        appender.write("");

        std::cout << "All writes completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

int main() {

    unitConsoleAppender();
    unitFileAppender();

    return 0;
}