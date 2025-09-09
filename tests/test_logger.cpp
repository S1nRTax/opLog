#include <iostream>
#include <memory>
#include "opLog/formatter/PlainTextFormatter.h"
#include "opLog/appender/ConsoleAppender.h"
#include "opLog/Logger.h"


int main() {

    auto formatter = std::make_unique<PlainTextFormatter>();
    auto appender = std::make_unique<ConsoleAppender>();
    Logger logger(std::move(formatter), std::move(appender), LogLevel::FATAL);

    logger.log(LogLevel::INFO, "This is a info message");
    logger.fatal("This is a fatal message");
    logger.info("Application started");
    logger.debug("This won't show (below INFO level)");
    logger.error("Something went wrong!");


    return 0;
}