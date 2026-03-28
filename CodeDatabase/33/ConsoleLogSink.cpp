#include "ConsoleLogSink.h"
#include <iostream>

void ConsoleLogSink::Write(const std::string& line) {
    std::cout << line << std::endl;
}
