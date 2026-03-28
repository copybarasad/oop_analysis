#pragma once
#include "position.h"
#include "spell.h" 

class VampireTower {
private:
    Position position_;
    int health_;
    int attackCooldown_;

public:
    VampireTower(const Position& position);
    
    void takeDamage(int amount);
    void decreaseCooldown();
    bool canAttack(const Position& hunterPosition) const;
    bool isReadyToAttack() const;
    
    Position getPosition() const;
    int getHealth() const;
    bool isAlive() const;
    
    SpellCastResult performAttack(const Position& target);
    
    //lb3
    void setHealth(int health) { health_ = health; }
    void setAttackCooldown(int cooldown) { attackCooldown_ = cooldown; }
};