#include "console_input_provider.h"
#include "command_handler.h"
#include <iostream>

std::unique_ptr<Command> ConsoleInputProvider::getNextCommand() {
    std::getline(std::cin, lastInput);
    return CommandHandler::parseCommand(lastInput);
}

void ConsoleInputProvider::showPrompt(const std::string& prompt) {
    std::cout << prompt;
    std::cout.flush();
}
