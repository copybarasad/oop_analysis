#include "GameSaveManager.h"
#include "ExceptionHandler.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

GameSaveManager::GameSaveManager() : currentSaveFile("") {
}

bool GameSaveManager::saveGame(const std::string& filename, const GameSaveData& data) {
    try {
        if (!validateSaveData(data)) {
            throw CorruptedSaveException("Invalid save data");
        }
        
        std::string fullPath = getSaveDirectory() + filename + ".sav";
        std::ofstream file(fullPath, std::ios::binary);
        
        if (!file.is_open()) {
            throw IOException("opening file " + fullPath + " for writing");
        }
        
        serializeGameData(file, data);
        file.close();
        
        return true;
    } catch (const std::exception& e) {
        throw SaveLoadException("Failed to save game: " + std::string(e.what()));
    }
}

bool GameSaveManager::loadGame(const std::string& filename, GameSaveData& data) {
    try {
        std::string fullPath = getSaveDirectory() + filename + ".sav";
        
        if (!fs::exists(fullPath)) {
            throw FileNotFoundException(fullPath);
        }
        
        std::ifstream file(fullPath, std::ios::binary);
        if (!file.is_open()) {
            throw IOException("opening file " + fullPath + " for reading");
        }
        
        if (!deserializeGameData(file, data)) {
            throw CorruptedSaveException("invalid file format");
        }
        
        file.close();
        
        if (!validateSaveData(data)) {
            throw CorruptedSaveException("data validation failed");
        }
        
        return true;
    } catch (const std::exception& e) {
        throw SaveLoadException("Failed to load game: " + std::string(e.what()));
    }
}

bool GameSaveManager::saveExists(const std::string& filename) const {
    std::string fullPath = getSaveDirectory() + filename + ".sav";
    return fs::exists(fullPath);
}

std::vector<std::string> GameSaveManager::getSaveFiles() const {
    std::vector<std::string> saves;
    std::string saveDir = getSaveDirectory();
    
    if (!fs::exists(saveDir)) {
        fs::create_directories(saveDir);
    }
    
    if (!fs::exists(saveDir)) {
        std::cerr << "Warning: Could not create saves directory: " << saveDir << std::endl;
        return saves;
    }
    
    try {
        for (const auto& entry : fs::directory_iterator(saveDir)) {
            if (entry.path().extension() == ".sav") {
                saves.push_back(entry.path().stem().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error reading saves directory: " << e.what() << std::endl;
    }
    
    return saves;
}

void GameSaveManager::serializeGameData(std::ofstream& file, const GameSaveData& data) {
    const char header[] = "HEROSAVE";
    file.write(header, 8);
    
    int version = 1;
    file.write(reinterpret_cast<const char*>(&version), sizeof(version));
    
    file.write(reinterpret_cast<const char*>(&data.playerHitpoints), sizeof(data.playerHitpoints));
    file.write(reinterpret_cast<const char*>(&data.playerMaxHitpoints), sizeof(data.playerMaxHitpoints));
    file.write(reinterpret_cast<const char*>(&data.playerPosition.row), sizeof(data.playerPosition.row));
    file.write(reinterpret_cast<const char*>(&data.playerPosition.col), sizeof(data.playerPosition.col));
    file.write(reinterpret_cast<const char*>(&data.meleeMode), sizeof(data.meleeMode));
    file.write(reinterpret_cast<const char*>(&data.enemiesDefeated), sizeof(data.enemiesDefeated));
    file.write(reinterpret_cast<const char*>(&data.mana), sizeof(data.mana));
    file.write(reinterpret_cast<const char*>(&data.currentLevel), sizeof(data.currentLevel));
    
    file.write(reinterpret_cast<const char*>(&data.boardWidth), sizeof(data.boardWidth));
    file.write(reinterpret_cast<const char*>(&data.boardHeight), sizeof(data.boardHeight));
    
    size_t enemyCount = data.enemies.size();
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    for (const auto& enemy : data.enemies) {
        file.write(reinterpret_cast<const char*>(&std::get<0>(enemy).row), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&std::get<0>(enemy).col), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&std::get<1>(enemy)), sizeof(int));
        file.write(reinterpret_cast<const char*>(&std::get<2>(enemy)), sizeof(int));
    }
    
    size_t towerCount = data.towers.size();
    file.write(reinterpret_cast<const char*>(&towerCount), sizeof(towerCount));
    for (const auto& tower : data.towers) {
        file.write(reinterpret_cast<const char*>(&std::get<0>(tower).row), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&std::get<0>(tower).col), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&std::get<1>(tower)), sizeof(int));
        file.write(reinterpret_cast<const char*>(&std::get<2>(tower)), sizeof(int));
        file.write(reinterpret_cast<const char*>(&std::get<3>(tower)), sizeof(int));
    }
    
    size_t spellCount = data.spellNames.size();
    file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
    for (const auto& spellName : data.spellNames) {
        size_t nameLength = spellName.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(spellName.c_str(), nameLength);
    }
}

bool GameSaveManager::deserializeGameData(std::ifstream& file, GameSaveData& data) {
    char header[9] = {0};
    file.read(header, 8);
    if (std::string(header) != "HEROSAVE") {
        return false;
    }
    
    int version;
    file.read(reinterpret_cast<char*>(&version), sizeof(version));
    if (version != 1) {
        return false;
    }
    
    file.read(reinterpret_cast<char*>(&data.playerHitpoints), sizeof(data.playerHitpoints));
    file.read(reinterpret_cast<char*>(&data.playerMaxHitpoints), sizeof(data.playerMaxHitpoints));
    file.read(reinterpret_cast<char*>(&data.playerPosition.row), sizeof(data.playerPosition.row));
    file.read(reinterpret_cast<char*>(&data.playerPosition.col), sizeof(data.playerPosition.col));
    file.read(reinterpret_cast<char*>(&data.meleeMode), sizeof(data.meleeMode));
    file.read(reinterpret_cast<char*>(&data.enemiesDefeated), sizeof(data.enemiesDefeated));
    file.read(reinterpret_cast<char*>(&data.mana), sizeof(data.mana));
    file.read(reinterpret_cast<char*>(&data.currentLevel), sizeof(data.currentLevel));
    
    file.read(reinterpret_cast<char*>(&data.boardWidth), sizeof(data.boardWidth));
    file.read(reinterpret_cast<char*>(&data.boardHeight), sizeof(data.boardHeight));
    
    size_t enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
    for (size_t i = 0; i < enemyCount; ++i) {
        Location pos;
        int hp, attack;
        file.read(reinterpret_cast<char*>(&pos.row), sizeof(pos.row));
        file.read(reinterpret_cast<char*>(&pos.col), sizeof(pos.col));
        file.read(reinterpret_cast<char*>(&hp), sizeof(hp));
        file.read(reinterpret_cast<char*>(&attack), sizeof(attack));
        data.enemies.emplace_back(pos, hp, attack);
    }
    
    size_t towerCount;
    file.read(reinterpret_cast<char*>(&towerCount), sizeof(towerCount));
    for (size_t i = 0; i < towerCount; ++i) {
        Location pos;
        int hp, attack, range;
        file.read(reinterpret_cast<char*>(&pos.row), sizeof(pos.row));
        file.read(reinterpret_cast<char*>(&pos.col), sizeof(pos.col));
        file.read(reinterpret_cast<char*>(&hp), sizeof(hp));
        file.read(reinterpret_cast<char*>(&attack), sizeof(attack));
        file.read(reinterpret_cast<char*>(&range), sizeof(range));
        data.towers.emplace_back(pos, hp, attack, range);
    }
    
    size_t spellCount;
    file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
    for (size_t i = 0; i < spellCount; ++i) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        std::string spellName(nameLength, '\0');
        file.read(&spellName[0], nameLength);
        data.spellNames.push_back(spellName);
    }
    
    return true;
}

bool GameSaveManager::validateSaveData(const GameSaveData& data) const {
    if (data.playerHitpoints < 0 || data.playerMaxHitpoints <= 0) return false;
    if (data.playerHitpoints > data.playerMaxHitpoints) return false;
    if (data.boardWidth == 0 || data.boardHeight == 0) return false;
    if (data.currentLevel <= 0) return false;
    if (data.mana < 0) return false;
    
    return true;
}

std::string GameSaveManager::getSaveDirectory() const {
    return "saves/";
}