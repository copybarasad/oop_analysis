#include "game_input_handler.h"
#include "exceptions.h"
#include <iostream>

char GameInputHandler::getPlayerCommand() {
    std::cout << "\nEnter command: ";
    char command;
    if (!(std::cin >> command)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read player command: input stream error");
    }
    return command;
}

std::pair<int, int> GameInputHandler::getSpellTarget() {
    int x, y;
    std::cout << "Enter target X: ";
    if (!(std::cin >> x)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read spell target X coordinate: input stream error");
    }
    
    std::cout << "Enter target Y: ";
    if (!(std::cin >> y)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read spell target Y coordinate: input stream error");
    }
    return {x, y};
}

bool GameInputHandler::getYesNoChoice(const std::string& prompt) {
    std::cout << prompt;
    char choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read yes/no choice: input stream error");
    }
    return (choice == 'y' || choice == 'Y');
}

std::string GameInputHandler::getFilename(const std::string& prompt) {
    std::cout << prompt;
    std::string filename;
    if (!(std::cin >> filename)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read filename: input stream error");
    }
    if (filename.empty()) {
        throw InvalidOperationException("Filename cannot be empty");
    }
    return filename;
}