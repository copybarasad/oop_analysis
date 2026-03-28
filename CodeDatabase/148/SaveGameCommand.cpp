#include "SaveGameCommand.h"
#include "Game.h"
#include "MenuSystem.h"
#include <iostream>

/**
 * @file SaveGameCommand.cpp
 * @brief Команда сохранения игры (ИСПРАВЛЕНО - запрашивает имя)
 */

// УДАЛИЛИ КОНСТРУКТОР - он уже определён в .h как inline

bool SaveGameCommand::execute(Game* game) {
    if (!game) {
        std::cout << "Error: Game is null\n";
        return false;
    }
    
    std::string name = saveName;
    
    // Если имя не задано - запрашиваем
    if (name.empty()) {
        name = MenuSystem::askSaveName();
    }
    
    try {
        game->saveGame(name);
        std::cout << "Game saved to: saves/" << name << ".sav\n";
        return true;
    } catch (const std::exception& e) {
        std::cout << "Failed to save game: " << e.what() << "\n";
        return false;
    }
}

std::string SaveGameCommand::getDescription() const {
    return "Save the game";
}


