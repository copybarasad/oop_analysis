#include "lib/SaveSystem.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#endif

const std::string SaveSystem::SAVE_FILE_PATH = "saves/";

bool createSaveDirectory() {
    const std::string save_path = "saves/";
#ifdef _WIN32
    return CreateDirectoryA(save_path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
#else
    return mkdir(save_path.c_str(), 0755) == 0 || errno == EEXIST;
#endif
}

bool SaveSystem::saveGame(const GameSaveData& data, const std::string& slotName) {
    if (!createSaveDirectory()) {
        std::cout << "Error: Could not create save directory!\n";
        return false;
    }
    
    std::string filename = SAVE_FILE_PATH + slotName + ".sav";
    std::ofstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        std::cout << "Error: Could not create save file!\n";
        return false;
    }
    
    try {
        // Player data
        size_t nameLength = data.playerName.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(data.playerName.c_str(), nameLength);
        
        file.write(reinterpret_cast<const char*>(&data.playerHealth), sizeof(data.playerHealth));
        file.write(reinterpret_cast<const char*>(&data.playerMaxHealth), sizeof(data.playerMaxHealth));
        file.write(reinterpret_cast<const char*>(&data.playerDamage), sizeof(data.playerDamage));
        file.write(reinterpret_cast<const char*>(&data.playerScore), sizeof(data.playerScore));
        file.write(reinterpret_cast<const char*>(&data.playerLevel), sizeof(data.playerLevel));
        file.write(reinterpret_cast<const char*>(&data.playerX), sizeof(data.playerX));
        file.write(reinterpret_cast<const char*>(&data.playerY), sizeof(data.playerY));
        file.write(reinterpret_cast<const char*>(&data.playerMana), sizeof(data.playerMana));
        file.write(reinterpret_cast<const char*>(&data.playerMaxMana), sizeof(data.playerMaxMana));
        
        // Field data
        file.write(reinterpret_cast<const char*>(&data.fieldWidth), sizeof(data.fieldWidth));
        file.write(reinterpret_cast<const char*>(&data.fieldHeight), sizeof(data.fieldHeight));
        
        for (int y = 0; y < data.fieldHeight; ++y) {
            for (int x = 0; x < data.fieldWidth; ++x) {
                bool passable = data.fieldPassable[y][x];
                bool hasEnemy = data.fieldHasEnemy[y][x];
                bool hasTower = data.fieldHasTower[y][x];
                
                file.write(reinterpret_cast<const char*>(&passable), sizeof(bool));
                file.write(reinterpret_cast<const char*>(&hasEnemy), sizeof(bool));
                file.write(reinterpret_cast<const char*>(&hasTower), sizeof(bool));
            }
        }
        
        // Enemies data
        size_t enemyCount = data.enemies.size();
        file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
        for (const auto& enemy : data.enemies) {
            file.write(reinterpret_cast<const char*>(&enemy.health), sizeof(enemy.health));
            file.write(reinterpret_cast<const char*>(&enemy.maxHealth), sizeof(enemy.maxHealth));
            file.write(reinterpret_cast<const char*>(&enemy.damage), sizeof(enemy.damage));
            file.write(reinterpret_cast<const char*>(&enemy.x), sizeof(enemy.x));
            file.write(reinterpret_cast<const char*>(&enemy.y), sizeof(enemy.y));
            file.write(reinterpret_cast<const char*>(&enemy.alive), sizeof(enemy.alive));
        }
        
        // Towers data
        size_t towerCount = data.towers.size();
        file.write(reinterpret_cast<const char*>(&towerCount), sizeof(towerCount));
        for (const auto& tower : data.towers) {
            file.write(reinterpret_cast<const char*>(&tower.health), sizeof(tower.health));
            file.write(reinterpret_cast<const char*>(&tower.maxHealth), sizeof(tower.maxHealth));
            file.write(reinterpret_cast<const char*>(&tower.x), sizeof(tower.x));
            file.write(reinterpret_cast<const char*>(&tower.y), sizeof(tower.y));
            file.write(reinterpret_cast<const char*>(&tower.attackRange), sizeof(tower.attackRange));
            file.write(reinterpret_cast<const char*>(&tower.attackDamage), sizeof(tower.attackDamage));
            file.write(reinterpret_cast<const char*>(&tower.alive), sizeof(tower.alive));
        }
        
        // Game state
        file.write(reinterpret_cast<const char*>(&data.currentTurn), sizeof(data.currentTurn));
        file.write(reinterpret_cast<const char*>(&data.currentLevel), sizeof(data.currentLevel));
        file.write(reinterpret_cast<const char*>(&data.gameRunning), sizeof(data.gameRunning));
        
        file.close();
        std::cout << "Game saved successfully to slot: " << slotName << "\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Error saving game: " << e.what() << "\n";
        return false;
    }
}

bool SaveSystem::loadGame(GameSaveData& data, const std::string& slotName) {
    std::string filename = SAVE_FILE_PATH + slotName + ".sav";
    std::ifstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        std::cout << "Error: Could not open save file!\n";
        return false;
    }
    
    try {
        // Player data
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        data.playerName.resize(nameLength);
        file.read(&data.playerName[0], nameLength);

        file.read(reinterpret_cast<char*>(&data.playerHealth), sizeof(data.playerHealth));
        file.read(reinterpret_cast<char*>(&data.playerMaxHealth), sizeof(data.playerMaxHealth));
        file.read(reinterpret_cast<char*>(&data.playerDamage), sizeof(data.playerDamage));
        file.read(reinterpret_cast<char*>(&data.playerScore), sizeof(data.playerScore));
        file.read(reinterpret_cast<char*>(&data.playerLevel), sizeof(data.playerLevel));
        file.read(reinterpret_cast<char*>(&data.playerX), sizeof(data.playerX));
        file.read(reinterpret_cast<char*>(&data.playerY), sizeof(data.playerY));
        file.read(reinterpret_cast<char*>(&data.playerMana), sizeof(data.playerMana));
        file.read(reinterpret_cast<char*>(&data.playerMaxMana), sizeof(data.playerMaxMana));
        
        // Field data
        file.read(reinterpret_cast<char*>(&data.fieldWidth), sizeof(data.fieldWidth));
        file.read(reinterpret_cast<char*>(&data.fieldHeight), sizeof(data.fieldHeight));
        
        data.fieldPassable.resize(data.fieldHeight);
        data.fieldHasEnemy.resize(data.fieldHeight);
        data.fieldHasTower.resize(data.fieldHeight);
        
        for (int y = 0; y < data.fieldHeight; ++y) {
            data.fieldPassable[y].resize(data.fieldWidth);
            data.fieldHasEnemy[y].resize(data.fieldWidth);
            data.fieldHasTower[y].resize(data.fieldWidth);
            
            for (int x = 0; x < data.fieldWidth; ++x) {
                bool passable, hasEnemy, hasTower;
                file.read(reinterpret_cast<char*>(&passable), sizeof(bool));
                file.read(reinterpret_cast<char*>(&hasEnemy), sizeof(bool));
                file.read(reinterpret_cast<char*>(&hasTower), sizeof(bool));
                
                data.fieldPassable[y][x] = passable;
                data.fieldHasEnemy[y][x] = hasEnemy;
                data.fieldHasTower[y][x] = hasTower;
            }
        }
        
        // Enemies data
        size_t enemyCount;
        file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
        data.enemies.resize(enemyCount);
        for (size_t i = 0; i < enemyCount; ++i) {
            GameSaveData::EnemyData enemyData;
            file.read(reinterpret_cast<char*>(&enemyData.health), sizeof(enemyData.health));
            file.read(reinterpret_cast<char*>(&enemyData.maxHealth), sizeof(enemyData.maxHealth));
            file.read(reinterpret_cast<char*>(&enemyData.damage), sizeof(enemyData.damage));
            file.read(reinterpret_cast<char*>(&enemyData.x), sizeof(enemyData.x));
            file.read(reinterpret_cast<char*>(&enemyData.y), sizeof(enemyData.y));
            file.read(reinterpret_cast<char*>(&enemyData.alive), sizeof(enemyData.alive));
            data.enemies[i] = enemyData;
        }
        
        // Towers data
        size_t towerCount;
        file.read(reinterpret_cast<char*>(&towerCount), sizeof(towerCount));
        data.towers.resize(towerCount);
        for (size_t i = 0; i < towerCount; ++i) {
            GameSaveData::TowerData towerData;
            file.read(reinterpret_cast<char*>(&towerData.health), sizeof(towerData.health));
            file.read(reinterpret_cast<char*>(&towerData.maxHealth), sizeof(towerData.maxHealth));
            file.read(reinterpret_cast<char*>(&towerData.x), sizeof(towerData.x));
            file.read(reinterpret_cast<char*>(&towerData.y), sizeof(towerData.y));
            file.read(reinterpret_cast<char*>(&towerData.attackRange), sizeof(towerData.attackRange));
            file.read(reinterpret_cast<char*>(&towerData.attackDamage), sizeof(towerData.attackDamage));
            file.read(reinterpret_cast<char*>(&towerData.alive), sizeof(towerData.alive));
            data.towers[i] = towerData;
        }
        
        // Game state
        file.read(reinterpret_cast<char*>(&data.currentTurn), sizeof(data.currentTurn));
        file.read(reinterpret_cast<char*>(&data.currentLevel), sizeof(data.currentLevel));
        file.read(reinterpret_cast<char*>(&data.gameRunning), sizeof(data.gameRunning));
        
        file.close();
        std::cout << "Game loaded successfully from slot: " << slotName << "\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Error loading game: " << e.what() << "\n";
        return false;
    }
}

std::vector<std::string> SaveSystem::getAvailableSaves() {
    std::vector<std::string> saves;
    
    if (!createSaveDirectory()) {
        return saves;
    }
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(SAVE_FILE_PATH)) {
            if (entry.path().extension() == ".sav") {
                saves.push_back(entry.path().stem().string());
            }
        }
    } catch (const std::filesystem::filesystem_error&) {
        // Папка не существует или другие ошибки файловой системы
        return saves;
    }
    
    return saves;
}

bool SaveSystem::deleteSave(const std::string& slotName) {
    std::string filename = SAVE_FILE_PATH + slotName + ".sav";
    return std::remove(filename.c_str()) == 0;
}

bool SaveSystem::saveExists(const std::string& slotName) {
    std::string filename = SAVE_FILE_PATH + slotName + ".sav";
    std::ifstream file(filename);
    return file.good();
}