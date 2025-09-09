#include <iostream>
#include "opLog/formatter/PlainTextFormatter.h"

int main() {

    const LogRecord record{LogLevel::DEBUG, "this is a debug message", std::chrono::system_clock::now()};

    PlainTextFormatter plainTextFormatter;
    std::cout << plainTextFormatter.format(record) << std::endl;

    return 0;
}