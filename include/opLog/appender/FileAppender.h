#ifndef FILEAPPENDER_H
#define FILEAPPENDER_H


#include "IAppender.h"
#include <iostream>

class FileAppender final : public IAppender {
    public:
    FileAppender() = default;
    ~FileAppender() override = default;
    void write(const std::string& message) override;
};

#endif //FILEAPPENDER_H
