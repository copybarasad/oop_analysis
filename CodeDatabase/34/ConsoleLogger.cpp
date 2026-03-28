#include "ConsoleLogger.h"
#include <iostream>

namespace Game {

void ConsoleLogger::log(const std::string& message) {
    std::cout << "[LOG]: " << message << std::endl;
}

}
