#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Player.h"
#include "Tower.h"
#include "AttackTower.h"
#include "Enemy.h"
#include "LevelManager.h"  

class Map;
class Player;
class Tower;
class AttackTower;
class Enemy;

class FileHandler {
private:
    std::fstream file;
    std::string filename;
    
public:
    FileHandler(const std::string& fname, std::ios_base::openmode mode) 
        : filename(fname) {
        file.open(filename, mode);
    }
    
    ~FileHandler() {
        if (file.is_open()) {
            file.close();
        }
    }
    
    std::fstream& get() { return file; }
    bool is_open() const { return file.is_open(); }
};

class SaveSystem {
private:
    std::string saveDirectory;
    
    void ProcessSection(const std::string& section, const std::string& data,
                       Map& map, Player& player, Tower& tower,
                       AttackTower& attackTower, int& counter, int& enemyCount, int& currentLevel);
    void ProcessEnemySection(const std::string& data, Map& map, int enemyIndex);

public:
    SaveSystem();
    bool SaveGame(const std::string& saveName, Map& map, Player& player,
                 Tower& tower, AttackTower& attackTower, int counter, int currentLevel);
    bool LoadGame(const std::string& saveName, Map& map, Player& player,
                 Tower& tower, AttackTower& attackTower, int& counter, int& currentLevel);
    std::vector<std::string> GetSaveList();
    bool SaveExists(const std::string& saveName);
    void DeleteSave(const std::string& saveName);
};