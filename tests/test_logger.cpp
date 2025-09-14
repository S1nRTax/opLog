#include "opLog/Logger.h"
#include "opLog/Config.h"


int main() {


        auto& logger = Logger::getInstance();
        for (int i{0}; i < 5000; ++i)
            logger.info("Hello World!" + std::to_string(i));
    return 0;
}
