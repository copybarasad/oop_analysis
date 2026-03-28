#include "game_state.h"
#include <iostream>
#include <stdexcept>

void GameState::saveToFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file for saving: " + filename);
    }
    
    file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
    
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cellValue = grid[y][x];
            file.write(reinterpret_cast<const char*>(&cellValue), sizeof(cellValue));
        }
    }
    
    
    file.write(reinterpret_cast<const char*>(&playerX), sizeof(playerX));
    file.write(reinterpret_cast<const char*>(&playerY), sizeof(playerY));
    file.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
    file.write(reinterpret_cast<const char*>(&playerMana), sizeof(playerMana));
    file.write(reinterpret_cast<const char*>(&playerScore), sizeof(playerScore));
    file.write(reinterpret_cast<const char*>(&playerCombatMode), sizeof(playerCombatMode));
    
    
    size_t enemyCount = enemies.size();
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    
    for (const auto& enemy : enemies) {
        file.write(reinterpret_cast<const char*>(&enemy.x), sizeof(enemy.x));
        file.write(reinterpret_cast<const char*>(&enemy.y), sizeof(enemy.y));
        file.write(reinterpret_cast<const char*>(&enemy.health), sizeof(enemy.health));
        file.write(reinterpret_cast<const char*>(&enemy.isAlive), sizeof(enemy.isAlive));
    }
    
    if (!file.good()) {
        throw std::runtime_error("Error writing to file: " + filename);
    }
    
    std::cout << "Game saved to: " << filename << std::endl;
}

void GameState::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file for loading: " + filename);
    }

    file.read(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
    
    
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));
    
    grid.resize(height, std::vector<int>(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cellValue;
            file.read(reinterpret_cast<char*>(&cellValue), sizeof(cellValue));
            grid[y][x] = cellValue;
        }
    }
    
    
    file.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
    file.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
    file.read(reinterpret_cast<char*>(&playerHealth), sizeof(playerHealth));
    file.read(reinterpret_cast<char*>(&playerMana), sizeof(playerMana));
    file.read(reinterpret_cast<char*>(&playerScore), sizeof(playerScore));
    file.read(reinterpret_cast<char*>(&playerCombatMode), sizeof(playerCombatMode));
    
    
    size_t enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
    
    enemies.clear();
    enemies.reserve(enemyCount);
    
    for (size_t i = 0; i < enemyCount; ++i) {
        EnemyData enemy;
        file.read(reinterpret_cast<char*>(&enemy.x), sizeof(enemy.x));
        file.read(reinterpret_cast<char*>(&enemy.y), sizeof(enemy.y));
        file.read(reinterpret_cast<char*>(&enemy.health), sizeof(enemy.health));
        file.read(reinterpret_cast<char*>(&enemy.isAlive), sizeof(enemy.isAlive));
        enemies.push_back(enemy);
    }
    
    if (!file.good()) {
        throw std::runtime_error("Error reading from file: " + filename);
    }
    
    validateState();
    std::cout << "Game loaded from: " << filename << std::endl;
}

void GameState::validateState() const {
    if (width < 10 || width > 25) throw std::runtime_error("Invalid width in save file");
    if (height < 10 || height > 25) throw std::runtime_error("Invalid height in save file");
    if (playerHealth < 0) throw std::runtime_error("Invalid player health in save file");
    if (playerMana < 0) throw std::runtime_error("Invalid player mana in save file");
    if (playerScore < 0) throw std::runtime_error("Invalid player score in save file");
    
    for (const auto& enemy : enemies) {
        if (enemy.health < 0) throw std::runtime_error("Invalid enemy health in save file");
    }
}