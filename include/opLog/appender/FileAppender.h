#ifndef FILEAPPENDER_H
#define FILEAPPENDER_H


#include "IAppender.h"
#include <iostream>

class FileAppender final : public IAppender {
    private:
        size_t currentFileSize;
        std::string generateFileName(const std::string& message);
        bool needsRotation(const std::string &filepath);

        void rotateFile(const std::string &filepath);

    public:
    FileAppender();
    ~FileAppender() override = default;
    void write(const std::string& message) override;
};

#endif //FILEAPPENDER_H
