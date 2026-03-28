#include "save_manager.hpp"
#include "game.hpp"
#include <iostream>
#include "player.hpp"
#include "level.hpp"
#include <fstream>
#include <system_error>

SaveManager::SaveManager(const std::string& filename) : saveFileName(filename) {}

bool SaveManager::saveGame(const Game& game) {
    try {
        std::ofstream file(saveFileName, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot create save file");
        }
        int currentLevel = game.getCurrentLevel();
        file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
        for (int i = 0; i <= currentLevel; i++) {
            const Level& level = game.getLevel(i);
            const Player& player = level.getPlayer();
            int hp = player.get_hp();
            int score = player.get_score();
            int x = player.get_x();
            int y = player.get_y();
            bool isRanged = player.getIsRangedCombat();
            file.write(reinterpret_cast<const char*>(&hp), sizeof(hp));
            file.write(reinterpret_cast<const char*>(&score), sizeof(score));
            file.write(reinterpret_cast<const char*>(&x), sizeof(x));
            file.write(reinterpret_cast<const char*>(&y), sizeof(y));
            file.write(reinterpret_cast<const char*>(&isRanged), sizeof(isRanged));
            const auto& enemies = level.getEnemies();
            int enemyCount = enemies.size();
            file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
            for (const auto& enemy : enemies) {
                int eHp = enemy.get_hp();
                int eX = enemy.get_x();
                int eY = enemy.get_y();
                bool eAlive = enemy.is_alive();
                file.write(reinterpret_cast<const char*>(&eHp), sizeof(eHp));
                file.write(reinterpret_cast<const char*>(&eX), sizeof(eX));
                file.write(reinterpret_cast<const char*>(&eY), sizeof(eY));
                file.write(reinterpret_cast<const char*>(&eAlive), sizeof(eAlive));
            }
        }
        file.close();
        std::cout << "=== GAME SAVED! ===" << std::endl;
        std::cout << "Full game state saved. You can exit safely." << std::endl;
        return true; 
    } catch (const std::exception& e) {
        std::cout << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::loadGame(Game& game) {
    try {
        std::ifstream file(saveFileName, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Save file not found");
        }
        int savedLevel;
        file.read(reinterpret_cast<char*>(&savedLevel), sizeof(savedLevel));
        game.setCurrentLevel(savedLevel);
        for (int i = 0; i <= savedLevel; i++) {
            Level& level = game.getLevel(i);
            Player& player = level.getPlayer();
            int hp, score, x, y;
            bool isRanged;
            file.read(reinterpret_cast<char*>(&hp), sizeof(hp));
            file.read(reinterpret_cast<char*>(&score), sizeof(score));
            file.read(reinterpret_cast<char*>(&x), sizeof(x));
            file.read(reinterpret_cast<char*>(&y), sizeof(y));
            file.read(reinterpret_cast<char*>(&isRanged), sizeof(isRanged));
            player.setHP(hp);
            player.setScore(score);
            player.setPosition(x, y);
            player.setCombatStyle(isRanged);
            int enemyCount;
            file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
            auto& enemies = level.getEnemies();
            enemies.clear();
            for (int j = 0; j < enemyCount; j++) {
                int eHp, eX, eY;
                bool eAlive;
                file.read(reinterpret_cast<char*>(&eHp), sizeof(eHp));
                file.read(reinterpret_cast<char*>(&eX), sizeof(eX));
                file.read(reinterpret_cast<char*>(&eY), sizeof(eY));
                file.read(reinterpret_cast<char*>(&eAlive), sizeof(eAlive));
                Enemy enemy(eX, eY);
                enemy.setHP(eHp);
                if (!eAlive) {
                    enemy.take_damage(eHp); 
                }
                enemies.push_back(enemy);
            }
        }
        file.close();
        std::cout << "=== GAME LOADED! ===" << std::endl;
        std::cout << "Full game state restored." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::saveExists() const {
    try {
        std::ifstream file(saveFileName);
        return file.good();
    } catch (const std::exception& e) {
        std::cout << "Error checking save file: " << e.what() << std::endl;
        return false;
    }
}

void SaveManager::deleteSave() {
    try {
        if (std::remove(saveFileName.c_str()) != 0) {
            return;
        }
    } catch (const std::exception& e) {
        std::cout << "Error deleting save file: " << e.what() << std::endl;
    }
}