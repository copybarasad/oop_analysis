#ifndef GAMESAVEMANAGER_H
#define GAMESAVEMANAGER_H

#include "GameField.h"
#include "HandSpell.h"
#include "SimpleFileSystem.h"
#include "Constants.h"
#include <string>
#include <vector>
#include <fstream>
#include <memory>

using namespace GameConstants;

class GameSaveManager {
private:
    std::string saveDirectory;
    
    void ensureSaveDirectoryExists();
    std::string getSaveFilePath(const std::string& saveName) const;
    
    void serializePlayer(std::ostream& os, const Player& player);
    void serializeGameField(std::ostream& os, const GameField& field);
    void serializeHandSpell(std::ostream& os, const HandSpell& handSpell);
    void serializeCharacter(std::ostream& os, const Character& character);
    void serializeBuilding(std::ostream& os, const Building& building, Type_cell buildingType);
    
    void deserializePlayer(std::istream& is, Player& player);
    void deserializeGameField(std::istream& is, GameField& field);
    void deserializeHandSpell(std::istream& is, HandSpell& handSpell);
    void deserializeCharacter(std::istream& is, Character& character);
    void deserializeBuilding(std::istream& is, Building& building);
    
    void serializePits(std::ostream& os, const GameField& field);
    void deserializePits(std::istream& is, GameField& field);

public:
    GameSaveManager(const std::string& directory = "saves");
    
    void saveGame(const std::string& saveName, const Player& player, const GameField& field, int currentTurn, int currentLevel);
    void loadGame(const std::string& saveName, Player& player, GameField& field, int& loadedTurn, int& loadedLevel);
    
    
    std::vector<std::string> getAvailableSaves() const;
    bool saveExists(const std::string& saveName) const;
};
#endif