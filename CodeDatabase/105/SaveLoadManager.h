#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

#include "Field.h"
#include "Player.h"
#include "GameVisualize.h"

#include <string>
#include <fstream>
#include <stdexcept>

class SaveLoadManager {
private:
    std::string saveFileName;
    ConsoleGameVisualizer visualizer;

public:
    SaveLoadManager(const std::string& filename = "savegame.dat");
    
    void saveGameWithLevel(const GameField& field, const Player& player, int turnCount, 
                          int currentLevel, int scoreForNextLevel);
    
    bool loadGameWithLevel(GameField& field, Player& player, int& turnCount,
                          int& currentLevel, int& scoreForNextLevel);
    
    
    bool saveExists() const;
    std::string getSaveInfo() const;

private:
    void validateSaveData(std::ifstream& file) const;
    void writeString(std::ofstream& file, const std::string& str) const;
    std::string readString(std::ifstream& file) const;
};

#endif