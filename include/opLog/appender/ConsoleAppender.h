#ifndef CONSOLE_APPENDER_H
#define CONSOLE_APPENDER_H

#include <iostream>
#include "IAppender.h"


class ConsoleAppender final : public IAppender {
    public:
    ConsoleAppender() = default;
    void write(const std::string& message) override;

};

#endif //CONSOLE_APPENDER_H
