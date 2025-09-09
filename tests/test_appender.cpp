#include<iostream>
#include "opLog/appender/ConsoleAppender.h"

int main() {

    ConsoleAppender appender;
    appender.write("Hello World!");
    appender.write("this is basic test for appender");


    return 0;
}