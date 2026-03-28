#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include "Position.h"
#include "Constants.h"
#include "GameException.h"
#include "CellType.h"

class GameState {
public:
    struct PlayerData {
        int health;
        int damage;
        Position position;
        int score;
        char fightingStyle;
        int attackRange;
        bool skipNextMove;
        int damageBonus;
        std::vector<std::string> spellNames;
        int buffCount;
    };
    
    struct UnitData {
        int type;
        int health;
        int damage;
        Position position;
        bool alive;
    };
    
    struct FieldCellData {
        int x, y;
        CellType cellType;
    };

    GameState();
    
    // Основные методы работы с файлами
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    
    const PlayerData& getPlayerData() const { return playerData; }
    const std::vector<UnitData>& getEnemiesData() const { return enemiesData; }
    const std::vector<UnitData>& getBuildingsData() const { return buildingsData; }
    const std::vector<UnitData>& getAlliesData() const { return alliesData; }
    const std::vector<FieldCellData>& getFieldData() const { return fieldData; }
    int getCurrentLevel() const { return currentLevel; }
    int getFieldSize() const { return fieldSize; }
    int getKillCount() const { return killCount; }
    
    void setPlayerData(const PlayerData& data) { playerData = data; }
    void setEnemiesData(const std::vector<UnitData>& data) { enemiesData = data; }
    void setBuildingsData(const std::vector<UnitData>& data) { buildingsData = data; }
    void setAlliesData(const std::vector<UnitData>& data) { alliesData = data; }
    void setFieldData(const std::vector<FieldCellData>& data) { fieldData = data; }
    void setCurrentLevel(int level) { currentLevel = level; }
    void setFieldSize(int size) { fieldSize = size; }
    void setKillCount(int count) { killCount = count; }

private:
    PlayerData playerData;
    std::vector<UnitData> enemiesData;
    std::vector<UnitData> buildingsData;
    std::vector<UnitData> alliesData;
    std::vector<FieldCellData> fieldData;
    int currentLevel;
    int fieldSize;
    int killCount;

    void serializeData(std::ofstream& file) const;
    void deserializeData(std::ifstream& file);
};

#endif