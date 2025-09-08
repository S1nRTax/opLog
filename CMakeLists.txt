cmake_minimum_required(VERSION 3.31)
project(opLog VERSION 1.0.0)


file(GLOB APPENDERS "src/appender/*.cpp") 

add_library(opLog
        src/logger.cpp
        src/formatter.cpp
        ${APPENDERS}
)

target_include_directories(
        opLog PUBLIC include
)

target_compile_features(opLog PUBLIC cxx_std_20)

add_executable(test_logger tests/test_logger.cpp)
add_executable(test_formatter tests/test_formatter.cpp)

target_link_libraries(test_logger PRIVATE opLog)
target_link_libraries(test_formatter PRIVATE opLog)