#include "SaveLoadManager.hpp"
#include <iostream>
#include <thread>
#include <chrono>

SaveLoadManager::SaveLoadManager(const std::string& filename) 
    : saveFileName(filename) {}

void SaveLoadManager::saveGame(const GameState& state) const {
    try {
        FileRAII file(saveFileName, std::ios::out | std::ios::binary);
        serializeGame(file.get(), state);
        
        if (file.get().fail()) {
            throw GameSaveException("Write operation failed");
        }
    }
    catch (const std::exception& e) {
        throw GameSaveException(std::string("Save failed: ") + e.what());
    }
}

std::unique_ptr<GameState> SaveLoadManager::loadGame() {
    try {
        FileRAII file(saveFileName, std::ios::in | std::ios::binary);
        
        file.get().seekg(0, std::ios::end);
        size_t fileSize = file.get().tellg();
        file.get().seekg(0, std::ios::beg);
        
        if (fileSize == 0) {
            throw GameCorruptedException("Файл сохранения пуст");
        }
        
        auto state = std::make_unique<GameState>(1);
        deserializeGame(file.get(), *state);
        
        if (file.get().fail()) {
            throw GameCorruptedException("Неверный формат файла сохранения");
        }
        
        return state;
    }
    catch (const std::exception& e) {
        throw GameLoadException(std::string("Load failed: ") + e.what());
    }
}

void SaveLoadManager::serializeGame(std::ostream& os, const GameState& state) const {
    const char* signature = "RPG1";
    os.write(signature, 4);
    
    int version = 1;
    os.write(reinterpret_cast<const char*>(&version), sizeof(version));
    
    os.write(reinterpret_cast<const char*>(&state.currentLevel), sizeof(state.currentLevel));
    os.write(reinterpret_cast<const char*>(&state.score), sizeof(state.score));
    
    state.player.serialize(os);
    
    size_t enemyCount = state.enemies.size();
    os.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    for (const auto& enemy : state.enemies) {
        enemy.serialize(os);
    }
    
    size_t allyCount = state.allies.size();
    os.write(reinterpret_cast<const char*>(&allyCount), sizeof(allyCount));
    for (const auto& ally : state.allies) {
        ally.serialize(os);
    }
    
    size_t buildingCount = state.buildings.size();
    os.write(reinterpret_cast<const char*>(&buildingCount), sizeof(buildingCount));
    for (const auto& building : state.buildings) {
        int buildingType = 0;
        if (dynamic_cast<const EnemyTower*>(building.get())) {
            buildingType = 1;
        } else if (dynamic_cast<const EnemyBuilding*>(building.get())) {
            buildingType = 2;
        }
        
        os.write(reinterpret_cast<const char*>(&buildingType), sizeof(buildingType));
        building->serialize(os);
    }
    
    size_t trapCount = state.traps.size();
    os.write(reinterpret_cast<const char*>(&trapCount), sizeof(trapCount));
    for (const auto& trap : state.traps) {
        trap.serialize(os);
    }
    
    state.field.serialize(os);
}

void SaveLoadManager::deserializeGame(std::istream& is, GameState& state) {
    char signature[4];
    is.read(signature, 4);
    if (std::string(signature, 4) != "RPG1") {
        throw GameCorruptedException("Неверная сигнатура файла сохранения");
    }
    
    int version;
    is.read(reinterpret_cast<char*>(&version), sizeof(version));
    if (version != 1) {
        throw GameCorruptedException("Неподдерживаемая версия сохранения");
    }
    
    is.read(reinterpret_cast<char*>(&state.currentLevel), sizeof(state.currentLevel));
    is.read(reinterpret_cast<char*>(&state.score), sizeof(state.score));
    
    state.player.deserialize(is);
    
    size_t enemyCount;
    is.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
    state.enemies.clear();
    for (size_t i = 0; i < enemyCount; ++i) {
        Enemy enemy(0, 0);
        enemy.deserialize(is);
        state.enemies.push_back(enemy);
    }
    
    size_t allyCount;
    is.read(reinterpret_cast<char*>(&allyCount), sizeof(allyCount));
    state.allies.clear();
    for (size_t i = 0; i < allyCount; ++i) {
        Ally ally(0, 0);
        ally.deserialize(is);
        state.allies.push_back(ally);
    }
    
    size_t buildingCount;
    is.read(reinterpret_cast<char*>(&buildingCount), sizeof(buildingCount));
    state.buildings.clear();
    for (size_t i = 0; i < buildingCount; ++i) {
        int buildingType;
        is.read(reinterpret_cast<char*>(&buildingType), sizeof(buildingType));
        
        std::unique_ptr<Building> building;
        switch (buildingType) {
            case 1:
                building = std::make_unique<EnemyTower>(0, 0);
                break;
            case 2:
                building = std::make_unique<EnemyBuilding>(0, 0);
                break;
            default:
                building = std::make_unique<EnemyBuilding>(0, 0);
                break;
        }
        
        building->deserialize(is);
        state.buildings.push_back(std::move(building));
    }
    
    size_t trapCount;
    is.read(reinterpret_cast<char*>(&trapCount), sizeof(trapCount));
    state.traps.clear();
    for (size_t i = 0; i < trapCount; ++i) {
        Trap trap(0, 0);
        trap.deserialize(is);
        state.traps.push_back(trap);
    }
    
    state.field.deserialize(is);
}