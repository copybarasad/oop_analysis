#pragma once
#include "GameBoard.h"
#include "Hero.h"
#include "Foe.h"
#include "EnemyTower.h"
#include "SpellHand.h"
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <tuple>
#include <filesystem> 

struct GameSaveData {
    int playerHitpoints;
    int playerMaxHitpoints;
    Location playerPosition;
    bool meleeMode;
    int enemiesDefeated;
    int mana;
    int currentLevel;

    size_t boardWidth;
    size_t boardHeight;

    std::vector<std::tuple<Location, int, int>> enemies; 
    std::vector<std::tuple<Location, int, int, int>> towers; 

    std::vector<std::string> spellNames;
};

class GameSaveManager {
private:
    std::string currentSaveFile;

public:
    GameSaveManager();
    
    bool saveGame(const std::string& filename, const GameSaveData& data);
    bool loadGame(const std::string& filename, GameSaveData& data);
    
    bool saveExists(const std::string& filename) const;
    std::vector<std::string> getSaveFiles() const;
    
    void setCurrentSave(const std::string& filename) { currentSaveFile = filename; }
    std::string getCurrentSave() const { return currentSaveFile; }

private:
    void serializeGameData(std::ofstream& file, const GameSaveData& data);
    bool deserializeGameData(std::ifstream& file, GameSaveData& data);
    bool validateSaveData(const GameSaveData& data) const;
    std::string getSaveDirectory() const;
};