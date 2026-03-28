#include "SaveLoadSystem.h"
#include "GameExceptions.h"
#include "SpellFactory.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include <iostream>
#include <filesystem>

const std::string SaveLoadSystem::DEFAULT_SAVE_FILE = "savegame.sav";
const unsigned int SaveLoadSystem::MAGIC_NUMBER;
const int SaveLoadSystem::SAVE_VERSION;

SaveLoadSystem::SaveLoadSystem() {
}

void SaveLoadSystem::SaveGame(const std::string& saveFilePath,
                               const Player& player,
                               const GameField& field,
                               const LevelManager& levelManager) {
    std::ofstream file(saveFilePath, std::ios::binary);
    
    if (!file.is_open()) {
        throw SaveFileWriteException(saveFilePath);
    }
    
    try {
        file.write(reinterpret_cast<const char*>(&MAGIC_NUMBER), sizeof(MAGIC_NUMBER));
        file.write(reinterpret_cast<const char*>(&SAVE_VERSION), sizeof(SAVE_VERSION));
        
        savePlayerData(file, player);
        saveFieldData(file, field);
        saveLevelData(file, levelManager);
        
        if (!file.good()) {
            throw SaveFileWriteException(saveFilePath);
        }
        
        file.close();
        
        std::cout << "\n✓ Игра успешно сохранена в файл: " << saveFilePath << std::endl;
    } catch (const SaveFileException&) {
        file.close();
        throw;
    } catch (const std::exception& error) {
        file.close();
        throw SaveFileWriteException(saveFilePath);
    }
}

void SaveLoadSystem::LoadGame(const std::string& saveFilePath,
                               Player& player,
                               GameField& field,
                               LevelManager& levelManager) {
    if (!SaveExists(saveFilePath)) {
        throw SaveFileNotFoundException(saveFilePath);
    }
    
    std::ifstream file(saveFilePath, std::ios::binary);
    
    if (!file.is_open()) {
        throw SaveFileReadException(saveFilePath);
    }
    
    try {
        validateMagicNumber(file, saveFilePath);
        validateVersion(file, saveFilePath);
        
        loadPlayerData(file, player);
        loadFieldData(file, field);
        loadLevelData(file, levelManager);
        
        if (!file.good() && !file.eof()) {
            throw SaveFileCorruptedException(saveFilePath, "ошибка чтения данных");
        }
        
        file.close();
        
        std::cout << "\n✓ Игра успешно загружена из файла: " << saveFilePath << std::endl;
    } catch (const SaveFileException&) {
        file.close();
        throw;
    } catch (const std::exception& error) {
        file.close();
        throw SaveFileCorruptedException(saveFilePath, error.what());
    }
}

bool SaveLoadSystem::SaveExists(const std::string& saveFilePath) const {
    return std::filesystem::exists(saveFilePath);
}

void SaveLoadSystem::savePlayerData(std::ofstream& file, const Player& player) {
    int health = player.GetHealth();
    int score = player.GetScore();
    CombatMode mode = player.GetCombatMode();
    size_t spellCount = player.GetSpellCount();
    
    file.write(reinterpret_cast<const char*>(&health), sizeof(health));
    file.write(reinterpret_cast<const char*>(&score), sizeof(score));
    file.write(reinterpret_cast<const char*>(&mode), sizeof(mode));
    file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
}

void SaveLoadSystem::saveFieldData(std::ofstream& file, const GameField& field) {
    int width = field.GetWidth();
    int height = field.GetHeight();
    int playerX = field.GetPlayerX();
    int playerY = field.GetPlayerY();
    size_t enemyCount = field.GetEnemyCount();
    size_t buildingCount = field.GetBuildingCount();
    size_t towerCount = field.GetTowerCount();
    
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    file.write(reinterpret_cast<const char*>(&playerX), sizeof(playerX));
    file.write(reinterpret_cast<const char*>(&playerY), sizeof(playerY));
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    file.write(reinterpret_cast<const char*>(&buildingCount), sizeof(buildingCount));
    file.write(reinterpret_cast<const char*>(&towerCount), sizeof(towerCount));
}

void SaveLoadSystem::saveLevelData(std::ofstream& file, const LevelManager& levelManager) {
    int levelNumber = levelManager.GetCurrentLevelNumber();
    file.write(reinterpret_cast<const char*>(&levelNumber), sizeof(levelNumber));
}

void SaveLoadSystem::loadPlayerData(std::ifstream& file, Player& player) {
	int health, score;
	CombatMode mode;
	size_t spellCount;
	
	file.read(reinterpret_cast<char*>(&health), sizeof(health));
	file.read(reinterpret_cast<char*>(&score), sizeof(score));
	file.read(reinterpret_cast<char*>(&mode), sizeof(mode));
	file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
	
	if (!file.good()) {
		throw std::runtime_error("не удалось прочитать данные игрока");
	}
	
	player.SetHealth(health);
	
	int currentScore = player.GetScore();
	if (score > currentScore) {
		player.AddScore(score - currentScore);
	}
}

void SaveLoadSystem::loadFieldData(std::ifstream& file, GameField& field) {
    int width, height, playerX, playerY;
    size_t enemyCount, buildingCount, towerCount;
    
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));
    file.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
    file.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
    file.read(reinterpret_cast<char*>(&buildingCount), sizeof(buildingCount));
    file.read(reinterpret_cast<char*>(&towerCount), sizeof(towerCount));
    
    if (!file.good()) {
        throw std::runtime_error("не удалось прочитать данные поля");
    }
    
    field.SetPlayerPosition(playerX, playerY);
}

void SaveLoadSystem::loadLevelData(std::ifstream& file, LevelManager& levelManager) {
    int levelNumber;
    file.read(reinterpret_cast<char*>(&levelNumber), sizeof(levelNumber));
    
    if (!file.good()) {
        throw std::runtime_error("не удалось прочитать данные уровня");
    }
    
    levelManager.SetCurrentLevel(levelNumber);
}

void SaveLoadSystem::validateMagicNumber(std::ifstream& file, const std::string& filename) {
    unsigned int magicNumber;
    file.read(reinterpret_cast<char*>(&magicNumber), sizeof(magicNumber));
    
    if (!file.good() || magicNumber != MAGIC_NUMBER) {
        throw SaveFileCorruptedException(filename, "неверная сигнатура файла");
    }
}

void SaveLoadSystem::validateVersion(std::ifstream& file, const std::string& filename) {
    int version;
    file.read(reinterpret_cast<char*>(&version), sizeof(version));
    
    if (!file.good()) {
        throw SaveFileCorruptedException(filename, "не удалось прочитать версию");
    }
    
    if (version != SAVE_VERSION) {
        throw SaveFileCorruptedException(filename, 
            "несовместимая версия (ожидается " + std::to_string(SAVE_VERSION) + 
            ", получена " + std::to_string(version) + ")");
    }
}

