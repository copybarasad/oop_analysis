#include "GameMenu.h"
#include "Game.h"
#include "GameController.h"
#include "GameVisualizer.h"
#include "ConsoleInputHandler.h"
#include "ConsoleRenderer.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <memory>

using namespace std;

void GameMenu::run() {
    bool programRunning = true;
    
    while (programRunning) {
        handleMainMenu();
    }
}

void GameMenu::handleMainMenu() {
    MenuOption choice = showMainMenu();
    
    unique_ptr<Game> gameInstance;
    
    switch(choice) {
        case NEW_GAME:
            gameInstance = make_unique<Game>();
            runGame(move(gameInstance));
            break;
            
        case LOAD_GAME: {
            string saveFile = showLoadGameMenu();
            if (!saveFile.empty()) {
                try {
                    gameInstance = make_unique<Game>();
                    gameInstance->loadGame(saveFile);
                    runGame(move(gameInstance));
                } catch (const exception& e) {
                    cout << "Error loading game: " << e.what() << endl;
                }
            }
            break;
        }
            
        case EXIT:
            exit(0);
            break;
    }
}

void GameMenu::handleGameOverMenu() {
    MenuOption choice = showGameOverMenu();
    
    switch(choice) {
        case NEW_GAME:
        case LOAD_GAME:
            break;
        case EXIT:
            exit(0);
            break;
    }
}

// Запуск игры
void GameMenu::runGame(unique_ptr<Game> game) {
    Game* gamePtr = game.get();
    
    if (!gamePtr) {
        return;
    }
    
    auto inputHandler = make_unique<ConsoleInputHandler>();
    GameController<ConsoleInputHandler> controller(*gamePtr, move(inputHandler));
    
    auto renderer = make_unique<ConsoleRenderer>();
    GameVisualizer<ConsoleRenderer> visualizer(*gamePtr, move(renderer));
    
    // Основной игровой цикл
    bool inGame = true;
    
    while (inGame && gamePtr->isGameRunning() && gamePtr->getPlayer().isAlive()) {
        visualizer.renderGameState();

        bool continueProcessing = controller.processNextCommand();
        
        if (!continueProcessing || controller.shouldReturnToMenu()) {
            inGame = false;
            break;
        }
        
        // Проверка конца игры
        if (gamePtr->checkGameEndCondition()) {
            inGame = false;
            
            visualizer.renderGameEnd();
            
            cout << endl;
            
            handleGameOverMenu();
            break;
        }
    }
    
    if (!gamePtr->isPlayerWon() && !gamePtr->getPlayer().isAlive()) {
        visualizer.renderGameEnd();
        
        cout << endl;
        
        handleGameOverMenu();
    }
}

GameMenu::MenuOption GameMenu::showMainMenu() {
    int choice;
    while (true) {
        cout << "=== MAIN MENU ===" << endl;
        cout << "1. New Game" << endl;
        cout << "2. Load Game" << endl;
        cout << "3. Exit" << endl;
        cout << "Choose: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        if (choice >= NEW_GAME && choice <= EXIT) {
            return static_cast<MenuOption>(choice);
        }
        cout << "Invalid choice! Please try again." << endl;
    }
}

GameMenu::MenuOption GameMenu::showGameOverMenu() {
    int choice;
    while (true) {
        cout << "\n=== GAME OVER ===" << endl;
        cout << "1. New Game" << endl;
        cout << "2. Load Saved Game" << endl;
        cout << "3. Exit" << endl;
        cout << "Choose: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        if (choice >= NEW_GAME && choice <= EXIT) {
            return static_cast<MenuOption>(choice);
        }
        cout << "Invalid choice! Please try again." << endl;
    }
}

std::vector<std::string> GameMenu::getSaveFiles() {
    std::vector<std::string> saveFiles;

    // Используем ANSI версию функций для работы с char*
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA("*.save", &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                saveFiles.push_back(string(findFileData.cFileName));
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
    
    std::sort(saveFiles.begin(), saveFiles.end());
    return saveFiles;
}

std::string GameMenu::showSaveGameMenu() {
    string filename;
    cout << "Enter save file name (without extension): ";
    cin >> filename;
    
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != string::npos) {
        filename = filename.substr(0, dotPos);
    }
    
    return filename + ".save";
}

std::string GameMenu::showLoadGameMenu() {
    auto saveFiles = getSaveFiles();
    
    if (saveFiles.empty()) {
        cout << "No save files found!" << endl;
        return "";
    }
    
    cout << "=== SAVED GAMES ===" << endl;
    for (size_t i = 0; i < saveFiles.size(); i++) {
        cout << i + 1 << ". " << saveFiles[i] << endl;
    }
    cout << "0. Cancel" << endl;
    
    int choice;
    cout << "Choose: ";
    
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input!" << endl;
        return "";
    }
    
    if (choice == 0 || choice > static_cast<int>(saveFiles.size())) {
        return "";
    }
    
    return saveFiles[choice - 1];
}