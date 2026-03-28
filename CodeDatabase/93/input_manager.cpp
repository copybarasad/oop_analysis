#include "input_manager.h"
#include <iostream>
#include <limits>
#include <string>

char InputManager::getInput() {
    std::string input;
    std::getline(std::cin, input);
    return (input.length() != 1) ? '\0' : input[0];
}

int InputManager::getFieldSize(const std::string& dimension) {
    int size;
    bool validInput = false;
    while (!validInput) {
        std::cout << "Enter field " << dimension << " (10-25): ";
        if (!(std::cin >> size)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (size >= 10 && size <= 25) {
            validInput = true;
        } else {
            std::cout << "Invalid size! Please enter a number between 10 and 25.\n";
        }
    }
    return size;
}

int InputManager::getEnemyCount() {
    int count;
    bool validInput = false;
    while (!validInput) {
        std::cout << "Enter number of enemies (3-5): ";
        if (!(std::cin >> count)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (count >= 3 && count <= 5) {
            validInput = true;
        } else {
            std::cout << "Invalid number! Please enter between 3 and 5 enemies.\n";
        }
    }
    return count;
}

void InputManager::waitForEnter() {
    std::cout << "\nPress Enter to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}