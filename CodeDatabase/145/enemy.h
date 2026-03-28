#ifndef ENEMY_H
#define ENEMY_H

#include "game_entity.h"
#include <utility>

class Enemy : public GameEntity {
private:
    std::pair<int, int> position;
    
public:
    Enemy(int health, int damage, int x, int y);
    
    std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);
    std::pair<int, int> calculateMoveTowardsPlayer(const std::pair<int, int>& playerPos) const;
};

#endif