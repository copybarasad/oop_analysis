#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include <chrono>
#include <thread>

#include "field.h"
#include "enemy_manager.h"
#include "player_manager.h"
#include "tower_manager.h"
#include "ispell.h"

class GeneralManager{
private:
    Field* field;
    EnemyManager* enemyMan;
    PlayerManager* playerMan;
    TowerManager* towerMan;

public:
    GeneralManager(int fieldWidth, int fieldHeight, int enemyCount, int towerCount=1, PlayerManager* updatePlayerMan = nullptr);
    GeneralManager();
    ~GeneralManager() = default;

    Field& getField();
    PlayerManager& getPlayerManager();
    EnemyManager& getEnemyManager();
    TowerManager& getTowerManager();

    
    void playerTurn(Direction direction);
    void enemiesTurn();
    void setField(Field* newField);
    void damageEnemyAt(int x, int y, int dmg);
    std::pair<int, int> moveEnemyRandom(int fromX, int fromY);
    std::pair<int, int> moveEnemyTowardsPlayer(int fromX, int fromY, std::pair<int, int> playerPos);

    void spawnPlayer(int x, int y);
    void spawnEnemyRandom(int health = 30, int damage = 5);
    void switchPlayerFightType();

    void addTower(Tower* tower);
    void checkTowerAttacks();

    std::pair<int, int> getRandomEmptyPosition() const;
    std::pair<int,int> getNear(std::pair<int, int> curPos, Direction dir);
    bool isFreePosition(int x, int y) const;


    void castSpell(int index, std::pair<int, int> target);
    const std::vector<ISpell*>& getSpells() const;
    void addPlayerSpell(ISpell* spell);
};