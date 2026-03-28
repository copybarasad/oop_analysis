#include "GameSaveSystem.h"
#include "GameField.h" // Добавили инклюд
#include <iostream>
#include <vector>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

bool GameSaveSystem::saveGame(const Game& game, const std::string& filename) {
    try {
        std::string fullFilename = filename;
        if (fullFilename.length() < 6 || fullFilename.substr(fullFilename.length() - 5) != ".save") {
            fullFilename += ".save";
        }
        
        std::ofstream file(fullFilename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot create save file: " + fullFilename);
        }
        
        writeInt(file, game.getCurrentTurn());
        writeBool(file, game.getIsGameActive());
        
        const Player& player = game.getPlayer();
        writeInt(file, player.getHitPoints());
        writeInt(file, player.getTotalScore());
        writeInt(file, player.getCoordX());
        writeInt(file, player.getCoordY());
        writeInt(file, player.getCurrentCombatStyle());
        
        const auto& enemies = game.getEnemies();
        writeInt(file, enemies.size());
        
        for (const auto& enemy : enemies) {
            writeInt(file, enemy.getHitPoints());
            writeInt(file, enemy.getCoordX());
            writeInt(file, enemy.getCoordY());
            writeBool(file, enemy.isAlive());
        }

        // === СОХРАНЕНИЕ ПОЛЯ ===
        const GameField& field = game.getField();
        writeInt(file, field.getFieldWidth());
        writeInt(file, field.getFieldHeight());
        
        for (int y = 0; y < field.getFieldHeight(); ++y) {
            for (int x = 0; x < field.getFieldWidth(); ++x) {
                writeBool(file, field.isObstacle(x, y));
            }
        }
        
        std::cout << "Game saved to: " << fullFilename << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Save error: " << e.what() << std::endl;
        return false;
    }
}

bool GameSaveSystem::loadGame(Game& game, const std::string& filename) {
    try {
        std::string fullFilename = filename;
        if (fullFilename.length() < 6 || fullFilename.substr(fullFilename.length() - 5) != ".save") {
            fullFilename += ".save";
        }
        
        if (!saveExists(fullFilename)) {
            throw std::runtime_error("Save file not found: " + fullFilename);
        }
        
        std::ifstream file(fullFilename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open save file: " + fullFilename);
        }
        
        int turn = readInt(file);
        bool isActive = readBool(file);
        
        int playerHP = readInt(file);
        int playerScore = readInt(file);
        int playerX = readInt(file);
        int playerY = readInt(file);
        CombatStyle playerStyle = static_cast<CombatStyle>(readInt(file));
        
        Player& player = game.getPlayer();
        player.receiveDamage(player.getHitPoints() - playerHP);
        player.addToScore(playerScore - player.getTotalScore());
        player.changePosition(playerX, playerY);
        while (player.getCurrentCombatStyle() != playerStyle) {
            player.toggleCombatStyle();
        }
        
        int enemyCount = readInt(file);
        std::vector<Enemy>& enemies = game.getEnemies();
        enemies.clear();
        
        for (int i = 0; i < enemyCount; ++i) {
            int enemyHP = readInt(file);
            int enemyX = readInt(file);
            int enemyY = readInt(file);
            bool enemyAlive = readBool(file);
            
            Enemy enemy(enemyX, enemyY);
            enemy.receiveDamage(enemy.getHitPoints() - enemyHP);
            if (!enemyAlive) {
                enemy.receiveDamage(1000);
            }
            enemies.push_back(enemy);
        }

        // === ЗАГРУЗКА ПОЛЯ ===
        int fieldW = readInt(file);
        int fieldH = readInt(file);
        
        GameField& field = const_cast<GameField&>(game.getField());
        
        for (int y = 0; y < fieldH; ++y) {
            for (int x = 0; x < fieldW; ++x) {
                bool isBlocked = readBool(file);
                field.setObstacle(x, y, isBlocked);
            }
        }

        game.setCurrentTurn(turn);
        
        std::cout << "Game loaded from: " << fullFilename << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Load error: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> GameSaveSystem::getSaveFiles() {
    std::vector<std::string> saves;
    
#ifdef _WIN32
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA("*.save", &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            saves.push_back(findData.cFileName);
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
#else
    DIR* dir = opendir(".");
    if (dir != nullptr) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename = entry->d_name;
            if (filename.length() > 5 && filename.substr(filename.length() - 5) == ".save") {
                saves.push_back(filename);
            }
        }
        closedir(dir);
    }
#endif
    
    return saves;
}

bool GameSaveSystem::saveExists(const std::string& filename) {
    std::ifstream file(filename);
    bool exists = file.good();
    file.close();
    return exists;
}

void GameSaveSystem::writeInt(std::ofstream& file, int value) {
    file.write(reinterpret_cast<const char*>(&value), sizeof(int));
}

int GameSaveSystem::readInt(std::ifstream& file) {
    int value;
    file.read(reinterpret_cast<char*>(&value), sizeof(int));
    return value;
}

void GameSaveSystem::writeBool(std::ofstream& file, bool value) {
    file.write(reinterpret_cast<const char*>(&value), sizeof(bool));
}

bool GameSaveSystem::readBool(std::ifstream& file) {
    bool value;
    file.read(reinterpret_cast<char*>(&value), sizeof(bool));
    return value;
}

void GameSaveSystem::writeString(std::ofstream& file, const std::string& str) {
    size_t length = str.length();
    file.write(reinterpret_cast<const char*>(&length), sizeof(size_t));
    file.write(str.c_str(), length);
}

std::string GameSaveSystem::readString(std::ifstream& file) {
    size_t length;
    file.read(reinterpret_cast<char*>(&length), sizeof(size_t));
    std::string str(length, ' ');
    file.read(&str[0], length);
    return str;
}