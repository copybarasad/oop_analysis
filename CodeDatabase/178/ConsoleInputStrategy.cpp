#include "ConsoleInputStrategy.h"
#include "GameController.h"
#include <iostream>

ConsoleInputStrategy::ConsoleInputStrategy() {
    try {
        loadConfigFromFile("controls.config");
    } catch (...) {
        std::cout << "Using default controls configuration." << std::endl;
    }
}

std::unique_ptr<ICommand> ConsoleInputStrategy::createCommand(char input, GameController& controller) {
    return commandFactory.createCommand(input, controller);
}

void ConsoleInputStrategy::loadConfig(const InputConfig& config) {
    std::cout << "ConsoleInputStrategy: Config loaded directly" << std::endl;
}

void ConsoleInputStrategy::showHelp() const {
    commandFactory.showHelp();
}

bool ConsoleInputStrategy::isValidInput(char input) const {
    return commandFactory.isValidInput(input);
}

void ConsoleInputStrategy::loadConfigFromFile(const std::string& filename) {
    commandFactory.loadConfig(filename);
}