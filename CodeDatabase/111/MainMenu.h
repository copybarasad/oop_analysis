#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "GameSession.h"
#include "GameController.h"
#include "SimpleInputStrategy.h"
#include <iostream>
#include <limits>
#include <filesystem>

namespace fs = std::filesystem;

class MainMenu {
private:
    void showMenu() const {
        std::cout << "\n"
            " ╔══════════════════════════════╗\n"
            " ║        HERO ADVENTURE        ║\n"
            " ╠══════════════════════════════╣\n"
            " ║  1. New Game                 ║\n"
            " ║  2. Load Game               ║\n"
            " ║  3. Show Save Files         ║\n"
            " ║  4. Exit                    ║\n"
            " ╚══════════════════════════════╝\n";
        std::cout << "Choose option: ";
    }
    
    void handleNewGame() {
        GameSession session;
        session.startNewGame();
        
        // Используем SimpleInputStrategy вместо DefaultInputStrategy
        GameController<SimpleInputStrategy> controller(&session);
        controller.run();
    }
    
    void handleLoadGame() {
        std::string filename;
        std::cout << "Enter save file name (without .sav): ";
        std::cin >> filename;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        GameSession session;
        if (session.loadGame(filename)) {
            std::cout << "Game loaded successfully!\n";
            
            GameController<SimpleInputStrategy> controller(&session);
            controller.run();
        } else {
            std::cout << "Failed to load game!\n";
        }
    }
    
    void showSaveFiles() const {
        std::string saveDir = "saves/";
        
        if (!fs::exists(saveDir)) {
            std::cout << "No saves directory found.\n";
            return;
        }
        
        std::cout << "\nAvailable save files:\n";
        bool found = false;
        
        try {
            for (const auto& entry : fs::directory_iterator(saveDir)) {
                if (entry.path().extension() == ".sav") {
                    std::cout << "  " << entry.path().stem().string() << "\n";
                    found = true;
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cout << "Error reading saves: " << e.what() << "\n";
        }
        
        if (!found) {
            std::cout << "  No save files found.\n";
        }
    }
    
public:
    void run() {
        std::cout << "=== Hero Adventure Game ===\n";
        std::cout << "Using Strategy Pattern Architecture\n\n";
        
        bool inMainMenu = true;
        
        while (inMainMenu) {
            showMenu();
            
            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    handleNewGame();
                    break;
                    
                case 2:
                    handleLoadGame();
                    break;
                    
                case 3:
                    showSaveFiles();
                    break;
                    
                case 4:
                    std::cout << "Thanks for playing! Goodbye!\n";
                    inMainMenu = false;
                    break;
                    
                default:
                    std::cout << "Invalid option! Please choose 1-4.\n";
                    break;
            }
        }
    }
};

#endif // MAIN_MENU_H