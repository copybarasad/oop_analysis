#pragma once

#include "Core/Map.h"
#include "Entities/Player.h"
#include "Entities/MasterCall.h"
#include "Enemies/Enemy.h"
#include "Enemies/EnemySpawner.h"
#include "Enemies/EnemyTower.h"
#include "Enemies/enemyAI.h"
#include "Spells/SpellHand.h"
#include "Spells/AimingSystem.h"
#include "Spells/SpellLvlUP.h"
#include "Animation/ASCIIAnimationPlayer.h"
#include "Protection/SaveFileProtector.h"
#include "Logger/GameLogger.h"
#include <vector>
#include <thread>
#include <chrono>

class GameMaster 
{
protected:
    std::vector<Enemy> enemies;
    std::vector<EnemySpawner> spawners;
    std::vector<EnemyTower> towers;
    std::vector<Coordinates> traps;
    std::vector<std::pair<Coordinates, int>> doppels;
    Player &player;
    int currentLevel;
    bool gameRunning;
    ASCIIAnimationPlayer animPlayer;
    
public:
    Map game_map;
    GameMaster(Player &player_ref);
    
    void initializeLevel(int level);
    void cleanupDeadEnemies();
    void reset();
    MasterCall processMoveRequest(Coordinates current, Coordinates target);
    void updateBuildings();
    void updateTraps();
    void updateDoppels();
    bool processSpellCast(SpellType spell, Coordinates target);
    void processPlayerAttack();
    void updateMap();
    void drawMap();
    std::vector<Enemy> &getEnemies();
    bool isGameRunning() const;
    void setGameRunning(bool running);
    int getCurrentLevel() const;
    bool checkLevelCompletion();
    void saveGame(const std::string& filename);
    bool loadGame(const std::string& filename);
    void showUpgradeMenu();
    void updateGameWorld();
};