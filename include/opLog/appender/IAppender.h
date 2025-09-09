#ifndef APPENDER_H
#define APPENDER_H
#include <string>

class IAppender {

public:
    virtual ~IAppender() = default;
    virtual void write(const std::string& msg) = 0; //todo: add thread safety
};

#endif //APPENDER_H
