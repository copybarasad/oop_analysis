#include "InputManager.hpp"
#include <iostream>

action InputManager::getPlayerAction() const {
    std::cout << "\nChoose action:\n0 - Move\n1 - Change Attack Type\n2 - Attack\n3 - Cast Spell\n";
    int choice; 
    
    while(true) {
        std::cin >> choice;
        
        if(std::cin.fail() || choice < 0 || choice > 3) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input! Please enter 0-3: ";
        } else {
            break;
        }
    }
    
    return static_cast<action>(choice);
}
