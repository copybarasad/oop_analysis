#include "SaveManager.h"
#include "GameEngine.h"
#include "GameField.h"
#include "EnemyTower.h"
#include <fstream>
#include <iostream>

void SaveManager::saveGame(const std::string& filename, const GameEngine* gameEngine, int currentLevel) {
    std::ofstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        throw SaveGameException("Could not create save file: " + filename);
    }
    
    try {
        file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
        
        if (gameEngine) {
            saveGameState(file, gameEngine);
        }
        
        if (file.fail()) {
            throw SaveGameException("Failed to write game data to file: " + filename);
        }
        
        file.close();
    } catch (const std::exception& e) {
        throw SaveGameException("Error during saving: " + std::string(e.what()));
    }
}

void SaveManager::loadGame(const std::string& filename, GameEngine* gameEngine, int& currentLevel) {
    std::ifstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        throw FileNotFoundException("Save file not found: " + filename);
    }
    
    try {
        file.read(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
        
        if (file.fail()) {
            throw CorruptedSaveException("File is corrupted or has invalid format: " + filename);
        }
        
        if (gameEngine) {
            loadGameState(file, gameEngine);
        }
        
        if (file.fail()) {
            throw CorruptedSaveException("File is corrupted or has invalid format: " + filename);
        }
        
        file.close();
    } catch (const CorruptedSaveException& e) {
        throw;
    } catch (const std::exception& e) {
        throw LoadGameException("Error during loading: " + std::string(e.what()));
    }
}

void SaveManager::saveGameState(std::ofstream& file, const GameEngine* gameEngine) {
    int health = gameEngine->player.getHealth();
    int score = gameEngine->player.getScore();
    int mana = gameEngine->player.getMana();
    CombatMode mode = gameEngine->player.getCombatMode();
    
    file.write(reinterpret_cast<const char*>(&health), sizeof(health));
    file.write(reinterpret_cast<const char*>(&score), sizeof(score));
    file.write(reinterpret_cast<const char*>(&mana), sizeof(mana));
    file.write(reinterpret_cast<const char*>(&mode), sizeof(mode));
    
    int spellCount = gameEngine->player.getSpellHand().getSpellCount();
    file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
    
    saveFieldState(file, &(gameEngine->field));
}

void SaveManager::loadGameState(std::ifstream& file, GameEngine* gameEngine) {
    int health, score, mana;
    CombatMode mode;
    
    file.read(reinterpret_cast<char*>(&health), sizeof(health));
    file.read(reinterpret_cast<char*>(&score), sizeof(score));
    file.read(reinterpret_cast<char*>(&mana), sizeof(mana));
    file.read(reinterpret_cast<char*>(&mode), sizeof(mode));
    
    gameEngine->player.setHealth(health);
    gameEngine->player.setScore(score);
    gameEngine->player.setMana(mana);
    gameEngine->player.setCombatMode(mode);
    
    int spellCount;
    file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
    
    loadFieldState(file, &(gameEngine->field));
}

void SaveManager::saveFieldState(std::ofstream& file, const GameField* field) {
    int width = field->getWidth();
    int height = field->getHeight();
    
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    
    Position playerPos = field->getPlayerPosition();
    int playerX = playerPos.getX();
    int playerY = playerPos.getY();
    file.write(reinterpret_cast<const char*>(&playerX), sizeof(playerX));
    file.write(reinterpret_cast<const char*>(&playerY), sizeof(playerY));
    
    const auto& grid = field->getGrid();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            CellType type = grid[y][x].getType();
            file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        }
    }
    
    const auto& enemies = field->getEnemies();
    const auto& enemyPositions = field->getEnemyPositions();
    
    size_t enemyCount = enemies.size();
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    
    for (size_t i = 0; i < enemyCount; ++i) {
        EnemyTower* tower = dynamic_cast<EnemyTower*>(enemies[i].get());
        int enemyType = tower ? 1 : 0;
        file.write(reinterpret_cast<const char*>(&enemyType), sizeof(enemyType));
        
        int health = enemies[i]->getHealth();
        int damage = enemies[i]->getDamage();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        
        int enemyX = enemyPositions[i].getX();
        int enemyY = enemyPositions[i].getY();
        file.write(reinterpret_cast<const char*>(&enemyX), sizeof(enemyX));
        file.write(reinterpret_cast<const char*>(&enemyY), sizeof(enemyY));
        
        if (tower) {
            int range = tower->getAttackRange();
            int cooldown = tower->getCooldown();
            file.write(reinterpret_cast<const char*>(&range), sizeof(range));
            file.write(reinterpret_cast<const char*>(&cooldown), sizeof(cooldown));
        }
    }
    
    const auto& trapPositions = field->getTrapPositions();
    const auto& trapDamages = field->getTrapDamages();
    
    size_t trapCount = trapPositions.size();
    file.write(reinterpret_cast<const char*>(&trapCount), sizeof(trapCount));
    
    for (size_t i = 0; i < trapCount; ++i) {
        int trapX = trapPositions[i].getX();
        int trapY = trapPositions[i].getY();
        int damage = trapDamages[i];
        
        file.write(reinterpret_cast<const char*>(&trapX), sizeof(trapX));
        file.write(reinterpret_cast<const char*>(&trapY), sizeof(trapY));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    }
}

void SaveManager::loadFieldState(std::ifstream& file, GameField* field) {
    int newWidth, newHeight;
    file.read(reinterpret_cast<char*>(&newWidth), sizeof(newWidth));
    file.read(reinterpret_cast<char*>(&newHeight), sizeof(newHeight));
    
    if (newWidth != field->getWidth() || newHeight != field->getHeight()) {
        throw CorruptedSaveException("Field size mismatch in save file");
    }
    
    int playerX, playerY;
    file.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
    file.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
    field->setPlayerPosition(Position(playerX, playerY));
    
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            CellType type;
            file.read(reinterpret_cast<char*>(&type), sizeof(type));
            field->setGridCell(x, y, type);
        }
    }
    
    size_t enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
    
    field->clearEnemies();
    
    for (size_t i = 0; i < enemyCount; ++i) {
        int enemyType;
        file.read(reinterpret_cast<char*>(&enemyType), sizeof(enemyType));
        
        int health, damage;
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        
        int enemyX, enemyY;
        file.read(reinterpret_cast<char*>(&enemyX), sizeof(enemyX));
        file.read(reinterpret_cast<char*>(&enemyY), sizeof(enemyY));
        
        Position enemyPos(enemyX, enemyY);
        
        if (enemyType == 1) {
            int range, cooldown;
            file.read(reinterpret_cast<char*>(&range), sizeof(range));
            file.read(reinterpret_cast<char*>(&cooldown), sizeof(cooldown));
            
            auto tower = std::make_unique<EnemyTower>(health, damage, range);
            tower->setCooldown(cooldown);
            field->addEnemyFromSave(std::move(tower), enemyPos);
        } else {
            field->addEnemyFromSave(std::make_unique<Enemy>(health, damage), enemyPos);
        }
    }
    
    size_t trapCount;
    file.read(reinterpret_cast<char*>(&trapCount), sizeof(trapCount));
    
    field->clearTraps();
    
    for (size_t i = 0; i < trapCount; ++i) {
        int trapX, trapY, damage;
        file.read(reinterpret_cast<char*>(&trapX), sizeof(trapX));
        file.read(reinterpret_cast<char*>(&trapY), sizeof(trapY));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        
        field->addTrapFromSave(Position(trapX, trapY), damage);
    }
}