#include "save_game.hpp"
#include "game_state.hpp"
#include "player.hpp"
#include "player_hand.hpp"
#include "enemy.hpp"
#include "enemy_building.hpp"
#include "game_exceptions.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <filesystem>

SaveGame::SaveGame(const std::string& filename) 
    : saveFileName(filename) {
}

bool SaveGame::save(const GameState& gameState) {
    try {
        SaveData data;
        serialize(gameState, data);
        
        std::ofstream file(saveFileName, std::ios::binary);
        if (!file) {
            throw WriteFileException(saveFileName);
        }
        
        if (!writeToFile(data)) {
            throw SaveGameException("Failed to write save data");
        }
        
        std::cout << "Игра сохранена в файл: " << saveFileName << "\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка сохранения: " << e.what() << "\n";
        return false;
    }
}

bool SaveGame::load(GameState& gameState) {
    try {
        if (!std::filesystem::exists(saveFileName)) {
            throw FileNotFoundException(saveFileName);
        }
        
        SaveData data;
        if (!readFromFile(data)) {
            throw CorruptSaveException("Invalid file format");
        }
        
        deserialize(gameState, data);
        
        std::cout << "Игра загружена из файла: " << saveFileName << "\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки: " << e.what() << "\n";
        return false;
    }
}

bool SaveGame::saveExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

std::vector<std::string> SaveGame::listSaves() {
    std::vector<std::string> saves;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(".")) {
            if (entry.path().extension() == ".sav" || 
                entry.path().filename() == "savegame.dat") {
                saves.push_back(entry.path().filename());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при поиске сохранений: " << e.what() << "\n";
    }
    
    return saves;
}

void SaveGame::serialize(const GameState& gameState, SaveData& data) const {
    if (auto player = gameState.getPlayer()) {
        data.playerX = player->getPositionX();
        data.playerY = player->getPositionY();
        data.playerHP = player->getHealthPoints();
        data.playerMaxHP = player->getMaxHealthPoints();
    }
    
    data.currentTurn = gameState.getCurrentTurn();
    data.currentLevel = gameState.getCurrentLevel();
    data.score = gameState.getScore();
    
    for (const auto& enemy : gameState.getEnemies()) {
        if (enemy) {
            data.enemyPositionsX.push_back(enemy->getPositionX());
            data.enemyPositionsY.push_back(enemy->getPositionY());
            data.enemyHP.push_back(enemy->getHealthPoints());
        }
    }
    
    for (const auto& building : gameState.getBuildings()) {
        if (building) {
            data.buildingPositionsX.push_back(building->getPositionX());
            data.buildingPositionsY.push_back(building->getPositionY());
            data.buildingHP.push_back(building->getHealthPoints());
        }
    }
}


void SaveGame::deserialize(GameState& gameState, const SaveData& data) const {
    gameState.cleanupGame();
    
    try {
        gameState.setCurrentTurn(data.currentTurn);
        gameState.setCurrentLevel(data.currentLevel);
        gameState.setScore(data.score);
        
        gameState.activateGame();
        
        gameState.createMapForLevel(data.currentLevel);
        
        Map* map = gameState.getMap();
        if (!map) {
            throw CorruptSaveException("Map creation failed");
        }
        
        if (data.playerHP > 0 && data.playerX >= 0 && data.playerY >= 0) {
            if (map->isValidPosition(data.playerX, data.playerY)) {
                Cell& playerCell = map->getCell(data.playerX, data.playerY);
                gameState.createPlayer(data.playerX, data.playerY, playerCell, 
                                       data.playerHP, data.playerMaxHP);
            }
        }
        
        Player* player = gameState.getPlayer();
        
        for (size_t i = 0; i < data.enemyPositionsX.size(); ++i) {
            if (data.enemyHP[i] > 0) {
                int x = data.enemyPositionsX[i];
                int y = data.enemyPositionsY[i];
                int hp = data.enemyHP[i];
                
                if (map->isValidPosition(x, y)) {
                    Cell& enemyCell = map->getCell(x, y);
                    if (enemyCell.isEmptyCell() && player) {
                        auto enemy = std::make_unique<Enemy>(
                            x, y, enemyCell, hp, 2, 10, player);
                        gameState.addEnemy(std::move(enemy));
                    }
                }
            }
        }
        
        for (size_t i = 0; i < data.buildingPositionsX.size(); ++i) {
            if (data.buildingHP[i] > 0) {
                int x = data.buildingPositionsX[i];
                int y = data.buildingPositionsY[i];
                int hp = data.buildingHP[i];
                
                if (map->isValidPosition(x, y)) {
                    Cell& buildingCell = map->getCell(x, y);
                    if (buildingCell.isEmptyCell() && player) {
                        auto& enemies = const_cast<std::vector<std::unique_ptr<GameObject>>&>(
                            gameState.getEnemies());
                        auto building = std::make_unique<EnemyBuilding>(
                            x, y, buildingCell, hp, 3, enemies, player);
                        gameState.addBuilding(std::move(building));
                    }
                }
            }
        }
        
    } catch (const std::exception& e) {
        throw CorruptSaveException(std::string("Deserialization error: ") + e.what());
    }
}

bool SaveGame::writeToFile(const SaveData& data) const {
    std::ofstream file(saveFileName, std::ios::binary);
    if (!file) return false;
    
    const char* header = "SAVEv1.0";
    file.write(header, 8);
    
    file.write(reinterpret_cast<const char*>(&data.currentTurn), sizeof(int));
    file.write(reinterpret_cast<const char*>(&data.currentLevel), sizeof(int));
    file.write(reinterpret_cast<const char*>(&data.score), sizeof(int));
    file.write(reinterpret_cast<const char*>(&data.playerX), sizeof(int));
    file.write(reinterpret_cast<const char*>(&data.playerY), sizeof(int));
    file.write(reinterpret_cast<const char*>(&data.playerHP), sizeof(int));
    file.write(reinterpret_cast<const char*>(&data.playerMaxHP), sizeof(int));
    
    int enemyCount = data.enemyPositionsX.size();
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(int));
    
    for (int i = 0; i < enemyCount; ++i) {
        file.write(reinterpret_cast<const char*>(&data.enemyPositionsX[i]), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.enemyPositionsY[i]), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.enemyHP[i]), sizeof(int));
    }
    
    int buildingCount = data.buildingPositionsX.size();
    file.write(reinterpret_cast<const char*>(&buildingCount), sizeof(int));
    
    for (int i = 0; i < buildingCount; ++i) {
        file.write(reinterpret_cast<const char*>(&data.buildingPositionsX[i]), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.buildingPositionsY[i]), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.buildingHP[i]), sizeof(int));
    }
    
    if (!file.good()) {
        throw WriteFileException(saveFileName);
    }
    
    return true;
}

bool SaveGame::readFromFile(SaveData& data) const {
    std::ifstream file(saveFileName, std::ios::binary);
    if (!file) return false;
    
    char header[9] = {0};
    file.read(header, 8);
    if (std::strcmp(header, "SAVEv1.0") != 0) {
        throw CorruptSaveException("Invalid file header");
    }
    
    file.read(reinterpret_cast<char*>(&data.currentTurn), sizeof(int));
    file.read(reinterpret_cast<char*>(&data.currentLevel), sizeof(int));
    file.read(reinterpret_cast<char*>(&data.score), sizeof(int));
    file.read(reinterpret_cast<char*>(&data.playerX), sizeof(int));
    file.read(reinterpret_cast<char*>(&data.playerY), sizeof(int));
    file.read(reinterpret_cast<char*>(&data.playerHP), sizeof(int));
    file.read(reinterpret_cast<char*>(&data.playerMaxHP), sizeof(int));
    
    int enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(int));
    
    if (enemyCount < 0 || enemyCount > 1000) {
        throw CorruptSaveException("Invalid enemy count");
    }
    
    data.enemyPositionsX.resize(enemyCount);
    data.enemyPositionsY.resize(enemyCount);
    data.enemyHP.resize(enemyCount);
    
    for (int i = 0; i < enemyCount; ++i) {
        file.read(reinterpret_cast<char*>(&data.enemyPositionsX[i]), sizeof(int));
        file.read(reinterpret_cast<char*>(&data.enemyPositionsY[i]), sizeof(int));
        file.read(reinterpret_cast<char*>(&data.enemyHP[i]), sizeof(int));
    }
    
    int buildingCount;
    file.read(reinterpret_cast<char*>(&buildingCount), sizeof(int));
    
    if (buildingCount < 0 || buildingCount > 100) {
        throw CorruptSaveException("Invalid building count");
    }
    
    data.buildingPositionsX.resize(buildingCount);
    data.buildingPositionsY.resize(buildingCount);
    data.buildingHP.resize(buildingCount);
    
    for (int i = 0; i < buildingCount; ++i) {
        file.read(reinterpret_cast<char*>(&data.buildingPositionsX[i]), sizeof(int));
        file.read(reinterpret_cast<char*>(&data.buildingPositionsY[i]), sizeof(int));
        file.read(reinterpret_cast<char*>(&data.buildingHP[i]), sizeof(int));
    }
    
    if (!file.good() && !file.eof()) {
        throw CorruptSaveException("File read error");
    }
    
    return true;
}