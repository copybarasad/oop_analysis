#include "SaveLoadManager.h"
#include "GameVisualize.h"
#include "AreaDamageSpell.h"
#include "DirectDamageSpell.h"
#include <iostream>

SaveLoadManager::SaveLoadManager(const std::string& filename) 
    : saveFileName(filename), visualizer() {}

void SaveLoadManager::saveGameWithLevel(const GameField& field, const Player& player, int turnCount,
                                       int currentLevel, int scoreForNextLevel) {
                                        
    std::ofstream file(saveFileName, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + saveFileName);
    }
    
    try {
        writeString(file, "GAME_SAVE_CORRECT");
        
        // уровни LevelManager
        file.write(reinterpret_cast<const char*>(&turnCount), sizeof(turnCount));
        file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
        file.write(reinterpret_cast<const char*>(&scoreForNextLevel), sizeof(scoreForNextLevel));
        
        // игрок
        int health = player.getHealth();
        int maxHealth = player.getMaxHealth();
        int mana = player.getMana();
        int maxMana = player.getMaxMana();
        int score = player.getScore();
        CombatType combatType = player.getCombatType();
        bool canMove = player.canMoveNextTurn();
        
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
        file.write(reinterpret_cast<const char*>(&mana), sizeof(mana));
        file.write(reinterpret_cast<const char*>(&maxMana), sizeof(maxMana));
        file.write(reinterpret_cast<const char*>(&score), sizeof(score));
        file.write(reinterpret_cast<const char*>(&combatType), sizeof(combatType));
        file.write(reinterpret_cast<const char*>(&canMove), sizeof(canMove));
        
        // заклинания
        const auto& spellHand = player.getSpellHand();
        size_t spellCount = spellHand.getSpellCount();
        file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
        
        for (size_t i = 0; i < spellCount; ++i) {
            Spell* spell = spellHand.getSpell(i);
            if (spell) {
                if (dynamic_cast<DirectDamageSpell*>(spell)) {
                    int spellType = 0;
                    file.write(reinterpret_cast<const char*>(&spellType), sizeof(spellType));
                } else if (dynamic_cast<AreaDamageSpell*>(spell)) {
                    int spellType = 1;
                    file.write(reinterpret_cast<const char*>(&spellType), sizeof(spellType));
                }
            }
        }
        
        // позиция игрока
        auto playerPos = field.getPlayerPosition();
        file.write(reinterpret_cast<const char*>(&playerPos.first), sizeof(playerPos.first));
        file.write(reinterpret_cast<const char*>(&playerPos.second), sizeof(playerPos.second));
        
        // размеры поля
        int width = field.getWidth();
        int height = field.getHeight();
        file.write(reinterpret_cast<const char*>(&width), sizeof(width));
        file.write(reinterpret_cast<const char*>(&height), sizeof(height));
        
        // клетки поля
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                CellType cellType = field.getCellType(x, y);
                bool hasPlayer = field.hasPlayer(x, y);
                bool hasEnemy = field.hasEnemy(x, y);
                bool hasBuilding = field.hasBuilding(x, y);
                
                file.write(reinterpret_cast<const char*>(&cellType), sizeof(cellType));
                file.write(reinterpret_cast<const char*>(&hasPlayer), sizeof(hasPlayer));
                file.write(reinterpret_cast<const char*>(&hasEnemy), sizeof(hasEnemy));
                file.write(reinterpret_cast<const char*>(&hasBuilding), sizeof(hasBuilding));
            }
        }
        
        // враги
        const auto& enemies = field.getEnemies();
        const auto& enemyPositions = field.getEnemyPositions();
        size_t enemyCount = enemies.size();
        file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
        

        for (const auto& enemy : enemies) {
            int enemyHealth = enemy.getHealth();
            int enemyDamage = enemy.getDamage();
            file.write(reinterpret_cast<const char*>(&enemyHealth), sizeof(enemyHealth));
            file.write(reinterpret_cast<const char*>(&enemyDamage), sizeof(enemyDamage));
        }
        
        // позиции врагов
        for (const auto& pos : enemyPositions) {
            file.write(reinterpret_cast<const char*>(&pos.first), sizeof(pos.first));
            file.write(reinterpret_cast<const char*>(&pos.second), sizeof(pos.second));
        }
        
        // здания
        const auto& buildings = field.getBuildings();
        const auto& buildingPositions = field.getBuildingPositions();
        size_t buildingCount = buildings.size();
        file.write(reinterpret_cast<const char*>(&buildingCount), sizeof(buildingCount));
        
        for (const auto& building : buildings) {
            int spawnCounter = building.getSpawnCounter();
            int spawnInterval = building.getSpawnInterval();
            file.write(reinterpret_cast<const char*>(&spawnCounter), sizeof(spawnCounter));
            file.write(reinterpret_cast<const char*>(&spawnInterval), sizeof(spawnInterval));
        }
        
        // позиции зданий
        for (const auto& pos : buildingPositions) {
            file.write(reinterpret_cast<const char*>(&pos.first), sizeof(pos.first));
            file.write(reinterpret_cast<const char*>(&pos.second), sizeof(pos.second));
        }
        
        if (!file.good()) {
            throw std::runtime_error("Error occurred during file writing");
        }
        
        visualizer.visualizeGameStateOperation("save", currentLevel);
        
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Save failed: ") + e.what());
    }
}

bool SaveLoadManager::loadGameWithLevel(GameField& field, Player& player, int& turnCount,
                                       int& currentLevel, int& scoreForNextLevel) {
    std::ifstream file(saveFileName, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Save file not found: " + saveFileName);
    }
    
    try {
        std::string header = readString(file);
        
        if (header == "GAME_SAVE_CORRECT") {
            file.read(reinterpret_cast<char*>(&turnCount), sizeof(turnCount));
            file.read(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
            file.read(reinterpret_cast<char*>(&scoreForNextLevel), sizeof(scoreForNextLevel));
            
        } else {
            throw std::runtime_error("Invalid or corrupted save file format: " + header);
        }
        
        // данные игрока
        int health, maxHealth, mana, maxMana, score;
        CombatType combatType;
        bool canMove;
        
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
        file.read(reinterpret_cast<char*>(&mana), sizeof(mana));
        file.read(reinterpret_cast<char*>(&maxMana), sizeof(maxMana));
        file.read(reinterpret_cast<char*>(&score), sizeof(score));
        file.read(reinterpret_cast<char*>(&combatType), sizeof(combatType));
        file.read(reinterpret_cast<char*>(&canMove), sizeof(canMove));
        
        // игрок
        player.setMaxHealth(maxHealth);
        player.setHealth(health);
        player.setMaxMana(maxMana);
        player.setMana(mana);
        player.setScore(score);
        player.setCombatType(combatType);
        if (!canMove) {
            player.setMovement(false);
        } else {
            player.resetMovement();
        }
        
        // заклинания
        player.clearSpells();
        size_t spellCount;
        file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
        
        for (size_t i = 0; i < spellCount; ++i) {
            int spellType;
            file.read(reinterpret_cast<char*>(&spellType), sizeof(spellType));
            
            std::unique_ptr<Spell> spell;
            switch (spellType) {
                case 0:
                    spell = std::make_unique<DirectDamageSpell>();
                    break;
                case 1:
                    spell = std::make_unique<AreaDamageSpell>();
                    break;
                default:
                    continue;
            }
            
            if (spell) {
                player.getSpellHand().addSpell(std::move(spell));
            }
        }
        
        // позиция игрока
        int playerX, playerY;
        file.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
        file.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
        
        // размеры поля
        int width, height;
        file.read(reinterpret_cast<char*>(&width), sizeof(width));
        file.read(reinterpret_cast<char*>(&height), sizeof(height));
        
        field = GameField(width, height);
        
        // клетки поля
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                CellType cellType;
                bool hasPlayer, hasEnemy, hasBuilding;
                
                file.read(reinterpret_cast<char*>(&cellType), sizeof(cellType));
                file.read(reinterpret_cast<char*>(&hasPlayer), sizeof(hasPlayer));
                file.read(reinterpret_cast<char*>(&hasEnemy), sizeof(hasEnemy));
                file.read(reinterpret_cast<char*>(&hasBuilding), sizeof(hasBuilding));
                
                field.getGrid()[y][x].setState(cellType, hasPlayer, hasEnemy, hasBuilding);
            }
        }
        
        // позиция игрока
        field.setInternalPlayerPosition(playerX, playerY);
        
        // враги
        size_t enemyCount;
        file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
        
        // убираем старых врагов
        field.getEnemies().clear();
        field.getEnemyPositionsRef().clear();
        
        // данные врагов
        for (size_t i = 0; i < enemyCount; ++i) {
            int enemyHealth, enemyDamage;
            file.read(reinterpret_cast<char*>(&enemyHealth), sizeof(enemyHealth));
            file.read(reinterpret_cast<char*>(&enemyDamage), sizeof(enemyDamage));
            
            Enemy enemy(enemyHealth, enemyDamage);
            enemy.setHealth(enemyHealth);
            field.getEnemies().push_back(enemy);
        }
        
        // позиции врагов
        for (size_t i = 0; i < enemyCount; ++i) {
            int x, y;
            file.read(reinterpret_cast<char*>(&x), sizeof(x));
            file.read(reinterpret_cast<char*>(&y), sizeof(y));
            field.getEnemyPositionsRef().push_back({x, y});
        }
        
        // здания
        size_t buildingCount;
        file.read(reinterpret_cast<char*>(&buildingCount), sizeof(buildingCount));
        
        // убираем старые здания
        field.getBuildingsRef().clear();
        field.getBuildingPositionsRef().clear();
        
        // данные зданий
        for (size_t i = 0; i < buildingCount; ++i) {
            int spawnCounter, spawnInterval;
            file.read(reinterpret_cast<char*>(&spawnCounter), sizeof(spawnCounter));
            file.read(reinterpret_cast<char*>(&spawnInterval), sizeof(spawnInterval));
            
            EnemyBuilding building(spawnInterval);
            building.setSpawnCounter(spawnCounter);
            field.getBuildingsRef().push_back(building);
        }
        
        //позиции зданий
        for (size_t i = 0; i < buildingCount; ++i) {
            int x, y;
            file.read(reinterpret_cast<char*>(&x), sizeof(x));
            file.read(reinterpret_cast<char*>(&y), sizeof(y));
            field.getBuildingPositionsRef().push_back({x, y});
        }
        
        if (!file.good() && !file.eof()) {
            throw std::runtime_error("Error occurred during file reading");
        }
        
        visualizer.visualizeGameStateOperation("load", currentLevel);
        return true;
        
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Load failed: ") + e.what());
    }
}

std::string SaveLoadManager::getSaveInfo() const {
    if (!saveExists()) {
        return "No save file found";
    }
    
    try {
        std::ifstream file(saveFileName, std::ios::binary);
        if (!file.is_open()) return "Cannot read save file";
        
        std::string header = readString(file);
        
        if (header == "GAME_SAVE_CORRECT") {
            int turnCount, currentLevel, scoreForNextLevel;
            file.read(reinterpret_cast<char*>(&turnCount), sizeof(turnCount));
            file.read(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
            file.read(reinterpret_cast<char*>(&scoreForNextLevel), sizeof(scoreForNextLevel));
            
            return "UNFINISHED GAME - Level " + std::to_string(currentLevel) + 
                   " - Turn: " + std::to_string(turnCount);
                   
        } else {
            return "Corrupted save file";
        }
        
    } catch (const std::exception& e) {
        return "Corrupted save file";
    }
}

bool SaveLoadManager::saveExists() const {
    std::ifstream file(saveFileName);
    return file.good();
}

void SaveLoadManager::validateSaveData(std::ifstream& file) const {
    std::string header = readString(file);
    if (header != "GAME_SAVE_CORRECT") {
        throw std::runtime_error("Invalid or corrupted save file format");
    }
}

void SaveLoadManager::writeString(std::ofstream& file, const std::string& str) const {
    size_t size = str.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(str.c_str(), size);
}

std::string SaveLoadManager::readString(std::ifstream& file) const {
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    
    if (size > 1000) {
        throw std::runtime_error("Invalid string size in save file");
    }
    
    std::string result(size, ' ');
    file.read(&result[0], size);
    return result;
}