#include "save_manager.h"
#include "game_controller.h"
#include <fstream>
#include <iostream>
#include <memory>

void SaveManager::saveGame(const GameController& game, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw SaveWriteException(filename);
    }
    try {
        game.save(file);
    } catch (...) {
        file.close();
        throw SaveWriteException(filename);
    }
    if (file.fail()) {
        file.close();
        throw SaveWriteException(filename);
    }
    file.close();
}

std::unique_ptr<GameController> SaveManager::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw SaveReadException(filename);
    }
    try {
        auto game = std::make_unique<GameController>();
        game->load(file);
        if (file.fail()) {
            throw SaveDataException("Stream error after loading");
        }
        file.close();
        return game;
    } catch (...) {
        file.close();
        throw SaveReadException(filename);
    }
}

bool SaveManager::saveExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}