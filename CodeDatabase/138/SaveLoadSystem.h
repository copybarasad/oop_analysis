#ifndef SAVE_LOAD_SYSTEM_H
#define SAVE_LOAD_SYSTEM_H

#include "Player.h"
#include "GameField.h"
#include "LevelManager.h"
#include <string>
#include <fstream>

class SaveLoadSystem {
public:
    SaveLoadSystem();
    
    void SaveGame(const std::string& saveFilePath, 
                  const Player& player, 
                  const GameField& field,
                  const LevelManager& levelManager);
    
    void LoadGame(const std::string& saveFilePath,
                  Player& player,
                  GameField& field,
                  LevelManager& levelManager);
    
    bool SaveExists(const std::string& saveFilePath) const;
    
    static const std::string DEFAULT_SAVE_FILE;
    static const int SAVE_VERSION = 1;

private:
    void savePlayerData(std::ofstream& file, const Player& player);
    void saveFieldData(std::ofstream& file, const GameField& field);
    void saveLevelData(std::ofstream& file, const LevelManager& levelManager);
    
    void loadPlayerData(std::ifstream& file, Player& player);
    void loadFieldData(std::ifstream& file, GameField& field);
    void loadLevelData(std::ifstream& file, LevelManager& levelManager);
    
    void validateMagicNumber(std::ifstream& file, const std::string& filename);
    void validateVersion(std::ifstream& file, const std::string& filename);
    
    static const unsigned int MAGIC_NUMBER = 0x4F4F5047;
};

#endif

