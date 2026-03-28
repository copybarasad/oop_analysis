#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "gamefield.h"
#include "enemy.h"
#include "enemytower.h"

enum class AttackType { MELEE, RANGED };

class CombatSystem {
private:
    int meleeDamage;
    int rangedDamage;
    AttackType currentAttackType;

public:
    CombatSystem(int meleeDmg, int rangedDmg);
    
    int getDamage() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    AttackType getAttackType() const;
    std::string getAttackTypeString() const;
    void switchAttackType();
    
    bool attackEnemy(const GameField& field, Enemy& enemy); 
};

#endif