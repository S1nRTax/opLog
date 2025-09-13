#include <iostream>
#include "opLog/formatter/PlainTextFormatter.h"

int main() {

    const LogRecord record{LogLevel::DEBUG, "this is a debug message", std::chrono::system_clock::now()};

    std::cout<< "Formatter with no brackets: \n";
    PlainTextFormatter plainTextFormatter(FormatStyle::STYLE_NO_BRACKETS);
    std::cout << plainTextFormatter.format(record) << std::endl;

    std::cout << "\nFormatter with brackets: \n";
    PlainTextFormatter plainTextFormatter2(FormatStyle::STYLE_WITH_BRACKETS);
    std::cout << plainTextFormatter2.format(record) << std::endl;

    return 0;
}