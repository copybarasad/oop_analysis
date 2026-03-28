#include "../../headers/ui/MenuManager.h"
#include "../../headers/gamelogic/Game.h"
#include <iostream>
#include <string>

using namespace std;

void MenuManager::showMainMenu() {
    cout << "=== MAIN MENU ===\n";
    cout << "1. New Game\n";
    cout << "2. Load Game\n";
    cout << "3. Exit\n";
    cout << "Choice: ";
}

int MenuManager::getMenuChoice() {
    string input;
    
    while (true) {
        getline(cin, input);
        
        try {
            int choice = stoi(input);
            return choice;
        } catch (...) {
            cout << "Invalid input! Please enter a number.\n";
        }
    }
}

void MenuManager::handleMainMenu() {
    while (true) {
        showMainMenu();
        int choice = getMenuChoice();
        
        switch(choice) {
            case 1:
                startNewGame();
                break;
            case 2:
                loadGame();
                break;
            case 3:
                cout << "Goodbye!\n";
                return;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void MenuManager::startNewGame() {
    Game game(15, 15, 1);
    game.runGame();
}

void MenuManager::loadGame() {
    try {
        cout << "Loading game..." << endl;
        Game game;
        game.loadGame();
        cout << "Game loaded successfully! Starting game..." << endl;
        game.runGame();
    }
    catch (const exception& e) {
        cout << "Failed to load game: " << e.what() << "\n";
        cout << "Starting new game instead...\n";
        startNewGame();
    }
}