#include "Game.h"
#include "PlayerLoader.h"
#include "FieldLoader.h"
#include "EntitiesLoader.h"
#include "GameExceptions.h"
#include <iostream>
#include <fstream>
#include <memory>

class FileRAII {
private:
    std::ifstream& file;
public:
    FileRAII(std::ifstream& f) : file(f) {}
    ~FileRAII() {
        if (file.is_open()) {
            file.close();
        }
    }
};

bool Game::loadGameFromFile(const std::string& filename) {
    try {
        std::cout << "Loading game from: " << filename << "\n";
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw FileException(filename, "open for reading");
        }
        
        FileRAII fileGuard(file);
        
        std::string header;
        if (!std::getline(file, header)) {
            throw LoadException("header", "cannot read header");
        }
        
        if (header != "BEAST_HUNTER_FULL_SAVE_V2") {
            throw ValidationException("file header", header);
        }

        currentGame = std::make_unique<GameController>(15, 15, true);
        
        if (!loadGameData(file, *currentGame)) {
            throw LoadException("game data", "one or more sections failed to load");
        }
        
        std::cout << "Game loaded successfully from: " << filename << "\n";
        return true;
        
    } catch (const FileException& e) {
        std::cout << "File error: " << e.what() << "\n";
        return false;
    } catch (const LoadException& e) {
        std::cout << "Load error: " << e.what() << "\n";
        return false;
    } catch (const ValidationException& e) {
        std::cout << "Validation error: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        std::cout << "Unexpected error during load: " << e.what() << "\n";
        return false;
    }
}

bool Game::loadGameData(std::ifstream& file, GameController& gameController) {
    try {
        if (!loadPlayerData(file, gameController)) {
            throw LoadException("player data", "failed to load player information");
        }
        
        if (!loadFieldData(file, gameController)) {
            throw LoadException("field data", "failed to load field layout");
        }
        
        if (!loadEntitiesData(file, gameController)) {
            throw LoadException("entities data", "failed to load game entities");
        }
        
        return true;
        
    } catch (const LoadException& e) {
        std::cout << "Game data loading failed: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        throw LoadException("game data", std::string("unexpected: ") + e.what());
    }
}

bool Game::validateHeader(std::ifstream& file) {
    std::string header;
    std::getline(file, header);
    if (header != "BEAST_HUNTER_FULL_SAVE_V2") {
        throw std::runtime_error("Invalid or corrupted save file: wrong header");
    }
    return true;
}

bool Game::loadPlayerData(std::ifstream& file, GameController& gameController) {
    PlayerLoader playerLoader(file, gameController.getPlayer(), 
                             gameController.getField(), gameController);
    return playerLoader.loadPlayerData();
}

bool Game::loadFieldData(std::ifstream& file, GameController& gameController) {
    FieldLoader fieldLoader(file, gameController.getField());
    return fieldLoader.loadFieldData();
}

bool Game::loadEntitiesData(std::ifstream& file, GameController& gameController) {
    EntitiesLoader entitiesLoader(file, gameController.getField(),
                                 gameController.getBeastManager(),
                                 gameController.getBuildingManager(),
                                 gameController);
    return entitiesLoader.loadEntitiesData();
}