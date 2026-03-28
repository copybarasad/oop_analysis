#include "QuitCommand.h"
#include "Game.h"
#include "MenuSystem.h"
#include <iostream>

/**
 * @file QuitCommand.cpp
 * @brief Команда выхода из игры (ИСПРАВЛЕНО)
 */

bool QuitCommand::execute(Game* game) {
    if (!game) {
        return false;
    }
    
    // Предлагаем сохраниться
    if (MenuSystem::askSaveBeforeExit()) {
        std::string saveName = MenuSystem::askSaveName();
        
        try {
            game->saveGame(saveName);
            std::cout << "Game saved to: saves/" << saveName << ".sav\n";
        } catch (const std::exception& e) {
            std::cout << "Failed to save: " << e.what() << "\n";
        }
    }
    
    // НЕ вызываем game->gameOver() - пусть main.cpp управляет
    return false; // Сигнал для выхода из цикла
}

std::string QuitCommand::getDescription() const {
    return "Quit the game";
}


