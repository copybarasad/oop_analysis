#include "SaveMenu.h"
#include <iostream>
#include <iomanip>
#include <limits>

SaveMenu::SaveMenu() {}

void SaveMenu::displaySaveSlots() const {
    std::cout << "\n=== SAVE SLOTS ===\n\n";
    
    for (int i = 1; i <= 3; i++) {
        std::cout << "Slot " << i << ":\n";
        std::cout << "  " << saveManager.getSaveInfo(i) << "\n";
    }
}

int SaveMenu::getSlotChoice() const {
    return InputHandler::getValidatedIntegerInput(
        "Select slot (1-3, 0 to cancel): ",
        0, 3);
}

bool SaveMenu::confirmOverwrite(int slot) const {
    if (!saveManager.saveExists(slot)) {
        return true;
    }
    
    std::cout << "\nSlot " << slot << " already has a save.\n";
    std::cout << "Current save: " << saveManager.getSaveInfo(slot) << "\n";
    std::cout << "Overwrite? (Y/N): ";
    
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return std::tolower(confirm) == 'y';
}

bool SaveMenu::showSaveMenu(const SaveData& data) {
    try {
        Renderer::clearScreen();
        std::cout << "=== SAVE GAME ===\n";
        
        displaySaveSlots();
        
        int choice = getSlotChoice();
        if (choice == 0) {
            std::cout << "\nSave cancelled.\n";
            InputHandler::waitForEnter();
            return false;
        }
        
        if (!confirmOverwrite(choice)) {
            std::cout << "\nSave cancelled.\n";
            InputHandler::waitForEnter();
            return false;
        }
        
        std::cout << "\nSaving game...\n";
        if (saveManager.saveGame(data, choice)) {
            std::cout << "Game saved successfully in slot " << choice << "!\n";
        } else {
            std::cout << "Failed to save game.\n";
            InputHandler::waitForEnter();
            return false;
        }
        
        InputHandler::waitForEnter();
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "\nError in save menu: " << e.what() << std::endl;
        InputHandler::waitForEnter();
        return false;
    }
}

bool SaveMenu::showLoadMenu(SaveData& data) {
    try {
        Renderer::clearScreen();
        std::cout << "=== LOAD GAME ===\n";
        
        displaySaveSlots();
        
        int choice = getSlotChoice();
        if (choice == 0) {
            std::cout << "\nLoad cancelled.\n";
            InputHandler::waitForEnter();
            return false;
        }
        
        if (!saveManager.saveExists(choice)) {
            std::cout << "\nNo save found in slot " << choice << "!\n";
            InputHandler::waitForEnter();
            return false;
        }
        
        std::cout << "\nLoading game...\n";
        if (saveManager.loadGame(data, choice)) {
            std::cout << "Game loaded successfully from slot " << choice << "!\n";
        } else {
            std::cout << "Failed to load game.\n";
            InputHandler::waitForEnter();
            return false;
        }
        
        InputHandler::waitForEnter();
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "\nError in load menu: " << e.what() << std::endl;
        InputHandler::waitForEnter();
        return false;
    }
}

void SaveMenu::showDeleteMenu() {
    try {
        Renderer::clearScreen();
        std::cout << "=== DELETE SAVE ===\n";
        
        displaySaveSlots();
        
        int choice = getSlotChoice();
        if (choice == 0) {
            std::cout << "Delete cancelled.\n";
            InputHandler::waitForEnter();
            return;
        }
        
        if (!saveManager.saveExists(choice)) {
            std::cout << "\nNo save found in slot " << choice << "!\n";
            InputHandler::waitForEnter();
            return;
        }
        
        std::cout << "\nAre you sure you want to delete save in slot " << choice << "? (Y/N): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (std::tolower(confirm) == 'y') {
            if (saveManager.deleteSave(choice)) {
                std::cout << "Save in slot " << choice << " deleted successfully.\n";
            } else {
                std::cout << "Failed to delete save.\n";
            }
        } else {
            std::cout << "Delete cancelled.\n";
        }
        
        InputHandler::waitForEnter();
        
    } catch (const std::exception& e) {
        std::cerr << "\nError in delete menu: " << e.what() << std::endl;
        InputHandler::waitForEnter();
    }
}

void SaveMenu::showSaveInfo() const {
    Renderer::clearScreen();
    std::cout << "=== SAVE INFORMATION ===\n\n";
    
    displaySaveSlots();
    
    InputHandler::waitForEnter();
}
