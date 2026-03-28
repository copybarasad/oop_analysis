#include "ConsoleInputHandler.h"
#include <iostream>

ConsoleInputHandler::ConsoleInputHandler() {
    setupDefaultBindings();
}

void ConsoleInputHandler::setupDefaultBindings() {
    keyBindings['w'] = std::make_unique<MoveUpCommand>();
    keyBindings['W'] = std::make_unique<MoveUpCommand>();
    keyBindings['s'] = std::make_unique<MoveDownCommand>();
    keyBindings['S'] = std::make_unique<MoveDownCommand>();
    keyBindings['a'] = std::make_unique<MoveLeftCommand>();
    keyBindings['A'] = std::make_unique<MoveLeftCommand>();
    keyBindings['d'] = std::make_unique<MoveRightCommand>();
    keyBindings['D'] = std::make_unique<MoveRightCommand>();
    keyBindings['c'] = std::make_unique<SwitchModeCommand>();
    keyBindings['C'] = std::make_unique<SwitchModeCommand>();
    keyBindings['q'] = std::make_unique<QuitCommand>();
    keyBindings['Q'] = std::make_unique<QuitCommand>();
}

std::unique_ptr<Command> ConsoleInputHandler::getNextCommand() {
    char input;
    std::cout << "\nEnter command (w/a/s/d/c/q): ";
    std::cin >> input;
    
    auto it = keyBindings.find(input);
    if (it != keyBindings.end()) {
        return std::unique_ptr<Command>(it->second->clone());
    }
    
    std::cout << "Invalid command! Valid commands: w, a, s, d, c, q\n";
    return nullptr;
}

void ConsoleInputHandler::setBinding(char key, std::unique_ptr<Command> command) {
    keyBindings[key] = std::move(command);
}