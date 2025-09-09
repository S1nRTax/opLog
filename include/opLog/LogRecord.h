#ifndef LOGRECORD_H
#define LOGRECORD_H

#include <chrono>
#include <string>
#include "LogLevel.h"

struct LogRecord {
    LogLevel logLevel;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
};


#endif //LOGRECORD_H
