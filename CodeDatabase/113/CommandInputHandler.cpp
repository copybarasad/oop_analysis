#include "CommandInputHandler.h"
#include "Command.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <string>

std::unique_ptr<ICommand> CommandInputHandler::getCommand() {
    char input = readCharInput();

    if (!isValidCommandChar(input)) {
        displayAvailableCommands();
        return nullptr;
    }

    return CommandFactory::createCommand(input);
}

char CommandInputHandler::readCharInput() {
    char input;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

bool CommandInputHandler::isValidCommandChar(char c) {
    const std::string validCommands = "wasdefcvlqi";
    return validCommands.find(static_cast<char>(std::tolower(static_cast<unsigned char>(c)))) != std::string::npos;
}

void CommandInputHandler::displayAvailableCommands() {
    std::cout << "\nAvailable commands:" << std::endl;
    std::cout << "  w, a, s, d - Move" << std::endl;
    std::cout << "  e - Attack" << std::endl;
    std::cout << "  f - Switch attack type" << std::endl;
    std::cout << "  c - Cast spell" << std::endl;
    std::cout << "  v - Save game" << std::endl;
    std::cout << "  l - Load game" << std::endl;
    std::cout << "  q - Quit game" << std::endl;
    std::cout << "  i - Show game info" << std::endl;
}