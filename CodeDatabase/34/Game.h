#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "Trap.h"
#include "observers/IObserver.h"
#include "observers/Event.h"
#include "spells/ISpell.h"
#include "../utils/Point.h"
#include "../utils/RandomGenerator.h" 
#include "commands/ICommand.h"

namespace Game {

enum class LevelResult {
    Win,
    Loss,
    Quit,
    SaveAndQuit,
    InProgress
};

class Game {
public:
    Game(int level, Player& playerRef, int scoreToWin);
    Game(std::istream& is, Player& playerRef);

    void runTurn(ICommand& command);

    void addObserver(IObserver* observer);
    void removeObserver(IObserver* observer);
    void notify(const Event& event);

    void movePlayer(int dr, int dc);
    void playerSwitchCombatMode();
    void playerCastSpell(int spellIndex, Utils::Point target);
    void setResult(LevelResult result);
    
    // Методы для системы логов
    void addLogMessage(const std::string& message);
    const std::vector<std::string>& getTurnLogs() const; 
    void clearTurnLogs();

    int getLevelNumber() const;
    int getScoreToWin() const;
    int getCurrentTurn() const;
    bool hasEnded() const;
    LevelResult getResult() const;
    const Map& getMap() const;
    const Player& getPlayer() const;
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;
    const std::vector<std::unique_ptr<EnemyBuilding>>& getEnemyBuildings() const;
    const std::vector<std::unique_ptr<EnemyTower>>& getEnemyTowers() const;
    const std::vector<std::unique_ptr<Ally>>& getAllies() const;
    const std::vector<std::unique_ptr<Trap>>& getTraps() const;

    Player& getPlayer();
    Enemy* getEnemyAtPosition(const Utils::Point& position);
    const Enemy* getEnemyAtPosition(const Utils::Point& position) const;
    EnemyBuilding* getBuildingAtPosition(const Utils::Point& position);
    const EnemyBuilding* getBuildingAtPosition(const Utils::Point& position) const;
    EnemyTower* getTowerAtPosition(const Utils::Point& position);
    const EnemyTower* getTowerAtPosition(const Utils::Point& position) const;
    Ally* getAllyAtPosition(const Utils::Point& position);
    const Ally* getAllyAtPosition(const Utils::Point& position) const;
    Trap* getTrapAtPosition(const Utils::Point& position);
    const Trap* getTrapAtPosition(const Utils::Point& position) const;
    
    void addTrap(std::unique_ptr<Trap> trap);
    void addAlly(std::unique_ptr<Ally> ally);
    bool isCellOccupied(const Utils::Point& position, bool checkPlayerAndAllies = true) const;
    std::vector<Utils::Point> getFreeAdjacentPositions(const Utils::Point& position) const;

    void serialize(std::ostream& os) const;

private:
    void deserialize(std::istream& is);
    void initializeNewLevel(int level);
    void processAITurns();

    void placePlayerRandomly();
    void placeEnemiesRandomly(int count, int health, int damage);
    void placeEnemyBuildingsRandomly(int count, int spawnInterval);
    void placeEnemyTowersRandomly(int count);

    void processAllyTurns();
    void processEnemyTurns();
    void processEnemyBuildingTurns();
    void processEnemyTowerTurns();
    void processTraps();
    
    void handlePlayerCombat(const Utils::Point& targetPosition);
    
    Utils::Point getNearestTargetDirection(const Utils::Point& from, const Utils::Point& to) const;
    void cleanupDefeated();

    bool isAdjacent(const Utils::Point& p1, const Utils::Point& p2) const;
    void spawnEnemy(const Utils::Point& buildingPosition);

    int levelNumber;
    int scoreToWin;
    int currentTurn;
    LevelResult resultState;

    std::unique_ptr<Map> gameMap;
    Player& player;
    
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<EnemyBuilding>> enemyBuildings;
    std::vector<std::unique_ptr<EnemyTower>> enemyTowers;
    std::vector<std::unique_ptr<Ally>> allies;
    std::vector<std::unique_ptr<Trap>> traps;

    std::vector<IObserver*> observers;
    std::vector<std::string> turnLogs;

    template<typename T>
    void serializeVector(std::ostream& os, const std::vector<std::unique_ptr<T>>& vec) const;
    template<typename T>
    void deserializeVector(std::istream& is, std::vector<std::unique_ptr<T>>& vec);
};

} 

#endif