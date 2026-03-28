#ifndef TOWER_H
#define TOWER_H

#include "position.h"
#include "direct_damage_spell.h"

class Tower {
private:
    Position position;
    int attackRange;
    int attackCooldown;
    int currentCooldown;
    DirectDamageSpell spell;
    
    static const int DEFAULT_ATTACK_RANGE = 2;    
    static const int DEFAULT_COOLDOWN = 5;        
    static const int DEFAULT_DAMAGE = 5;         

public:
    explicit Tower(int range = DEFAULT_ATTACK_RANGE, int cooldown = DEFAULT_COOLDOWN);
    
    Position getPosition() const;
    void setPosition(const Position& newPosition);
    
    bool canAttack() const;
    bool isInRange(const Position& targetPosition) const;
    void attack();
    void update();
    
    int getDamage() const;
    int getRange() const;
    int getCooldown() const;
    int getCurrentCooldown() const;
};

#endif