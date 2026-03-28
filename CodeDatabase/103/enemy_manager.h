#pragma once

#include <map>
#include <algorithm>
#include <vector>
#include "player.h"
#include "enemy.h"

class EnemyManager {
private:
    std::map<std::pair<int, int>, Enemy*> enemies;

public:
    EnemyManager(const std::vector<std::pair<int, int>>& initialPositions);
    ~EnemyManager();

    bool isEnemyAliveAt(int x, int y) const;
    void attackEnemyAt(int x, int y, int damage);
    void addEnemy(Enemy* enemy, int x, int y);
    Enemy* getEnemyAt(int x, int y) const;
    int getEnemyDamageAt(int x, int y) const;
    void moveEnemy(int fromX, int fromY, int toX, int toY);
    void removeEnemyAt(int x, int y);
    bool hasEnemyAt(int x, int y) const;
    std::vector<std::tuple<int, int, int, int>> getEnemiesData() const;
    std::vector<std::pair<int, int>> getAliveEnemyPositions() const;

};