#ifndef TOWER_TOWER_SPELL_H
#define TOWER_TOWER_SPELL_H

#include "game_entity.h"
#include <utility>

class TowerSpell : public GameEntity {
private:
    std::pair<int, int> position;
    int attackRadius;
    int attackCooldown;
    int currentCooldown;
    
public:
    TowerSpell(int x, int y, int health = 40, int damage = 5);
    std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);
    bool canAttack() const;
    void resetCooldown();
    void updateCooldown();
    int getAttackRadius() const;
    int getDamage() const;
    int getHealth() const;
    void takeDamage(int amount);
    bool isAlive() const;
};

#endif