#ifndef TOWER_H
#define TOWER_H

#include "spell.h"
#include <vector>
#include <memory>

class Player;
class Enemy;

class Tower {
public:
    Tower(int x, int y, int attackRange, int cooldown);
    
    void update(Player& player, std::vector<Enemy>& enemies);
    bool canAttack() const;
    void attack(Player& player, std::vector<Enemy>& enemies);
    
    int getX() const { return x; }
    int getY() const { return y; }
    int getAttackRange() const { return attackRange; }
    bool isActive() const { return active; }
    
private:
    int x, y;
    int attackRange;
    int cooldown;
    int currentCooldown;
    bool active;
    std::unique_ptr<Spell> attackSpell;
    
    bool isPlayerInRange(const Player& player) const;
    void createAttackSpell();
};

#endif