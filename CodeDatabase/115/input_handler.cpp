#include "input_handler.h"
#include <iostream>

InputHandler::InputHandler() {}

void InputHandler::loadConfig(const std::string& filename) {
    config.loadFromFile(filename);
}

Command InputHandler::readInput() {
    char input_char;
    std::cout << "Enter command: ";
    std::cin >> input_char;
    
    return config.getCommand(input_char);
}