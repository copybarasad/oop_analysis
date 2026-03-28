#include "ConsoleInput.h"
#include <iostream>


std::string ConsoleInput::Read() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

