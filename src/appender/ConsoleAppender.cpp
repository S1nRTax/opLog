#include "opLog/appender/ConsoleAppender.h"


void ConsoleAppender::write(const std::string& message) {
    std::cout << message << "\n";
}
