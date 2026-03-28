#ifndef ALLY_H
#define ALLY_H

#include "game_entity.h"
#include <utility>
#include <vector>


class Ally : public GameEntity {
private:
    std::pair<int, int> position;
    
public:
    Ally(int health, int damage, int x, int y);
    
    std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);
    std::pair<int, int> calculateMoveTowardsClosestEnemy(
        const std::vector<class Enemy>& enemies,
        const class GameField& field,
        const std::vector<class EnemyBuilding>& buildings,
        const std::vector<class EnemyTower>& towers) const;
};

#endif