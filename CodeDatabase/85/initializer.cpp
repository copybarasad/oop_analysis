#include "initializer.h"
#include <iostream>
#include <limits>

int Initializer::get_field_height() {
    int height;
    while (true) {
        std::cout << "Enter field height (10-25): ";
        std::cin >> height;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 10 and 25.\n";
        } else if (height >= 10 && height <= 25) {
            break;
        } else {
            std::cout << "Invalid input. Please enter a value between 10 and 25.\n";
        }
    }
    return height;
}

int Initializer::get_field_width() {
    int width;
    while (true) {
        std::cout << "Enter field width (10-25): ";
        std::cin >> width;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 10 and 25.\n";
        } else if (width >= 10 && width <= 25) {
            break;
        } else {
            std::cout << "Invalid input. Please enter a value between 10 and 25.\n";
        }
    }
    return width;
}

int Initializer::get_enemy_count() {
    int count;
    const int MAX_ENEMIES = 50;
    const int MIN_ENEMIES = 1;
    
    while (true) {
        std::cout << "Enter number of enemies (1-" << MAX_ENEMIES << "): ";
        std::cin >> count;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between " << MIN_ENEMIES << " and " << MAX_ENEMIES << ".\n";
        } else if (count >= MIN_ENEMIES && count <= MAX_ENEMIES) {
            break;
        } else {
            std::cout << "Invalid input. Please enter between " << MIN_ENEMIES << " and " << MAX_ENEMIES << ".\n";
        }
    }
    return count;
}