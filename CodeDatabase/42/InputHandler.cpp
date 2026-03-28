#include "InputHandler.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <sstream>

int InputHandler::getValidatedIntegerInput(const std::string& prompt, int minValue, int maxValue) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        bool isValidNumber = true;
        for (char c : input) {
            if (!std::isdigit(c)) {
                isValidNumber = false;
                break;
            }
        }

        if (!isValidNumber) {
            std::cout << "Invalid input. Please enter only digits." << std::endl;
            continue;
        }

        std::istringstream iss(input);
        int value;
        char extraChar;
        if (!(iss >> value) || (iss >> extraChar)) {
            std::cout << "Invalid input. Please enter a single number." << std::endl;
            continue;
        }

        if (value < minValue || value > maxValue) {
            std::cout << "Invalid input. Please enter a number between " 
                      << minValue << " and " << maxValue << "." << std::endl;
            continue;
        }

        return value;
    }
}

bool InputHandler::isValidCommandCharacter(char c) {
    char upperC = std::toupper(c);
    return upperC == 'W' || upperC == 'A' || upperC == 'S' || upperC == 'D' || 
           upperC == 'X' || upperC == 'T' || upperC == 'F' || upperC == 'R' || 
           upperC == 'Q' || upperC == 'K' || upperC == 'L';
}

char InputHandler::getGameInput() {
    std::string input;
    while (true) {
        std::cout << "Enter command: ";
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << "Invalid input. Please enter a command." << std::endl;
            continue;
        }

        char firstChar = std::toupper(input[0]);
        
        if (!isValidCommandCharacter(firstChar)) {
            std::cout << "Invalid command. Use: W/A/S/D (move), X (stay), T (switch attack), F (ranged), R (spells), Q (quit)." << std::endl;
            continue;
        }

        if (input.length() > MAX_COMMAND_LENGTH) {
            std::cout << "Error: Command is too long. Expected single character, got " 
                      << input.length() << " characters. Use only the first character." << std::endl;
            return firstChar;
        }

        return firstChar;
    }
}

void InputHandler::clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void InputHandler::waitForEnter() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Press Enter to continue...";
    std::string dummy;
    std::getline(std::cin, dummy);
}
