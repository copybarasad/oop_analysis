#include "CombatSystem.h"
#include <iostream>
#include <cmath>

CombatSystem::CombatSystem(int meleeDmg, int rangedDmg) 
    : meleeDamage(meleeDmg), rangedDamage(rangedDmg), currentAttackType(AttackType::MELEE) {}

int CombatSystem::getDamage() const { 
    return (currentAttackType == AttackType::MELEE) ? meleeDamage : rangedDamage; 
}

int CombatSystem::getMeleeDamage() const { return meleeDamage; }
int CombatSystem::getRangedDamage() const { return rangedDamage; }
AttackType CombatSystem::getAttackType() const { return currentAttackType; }

std::string CombatSystem::getAttackTypeString() const { 
    return (currentAttackType == AttackType::MELEE) ? "Ближний бой" : "Дальний бой"; 
}

void CombatSystem::switchAttackType() {
    currentAttackType = (currentAttackType == AttackType::MELEE) ? AttackType::RANGED : AttackType::MELEE;
}

bool CombatSystem::attackEnemy(const GameField& field, Enemy& enemy) {
    if (!enemy.isAlive()) { 
        std::cout << "Враг уже мертв!\n"; 
        return false; 
    }
    
    int playerX, playerY, enemyX, enemyY;
    if (!field.findPlayerPosition(playerX, playerY) || !field.findEnemyPosition(enemyX, enemyY)) {
        return false;
    }
    
    int distX = abs(playerX - enemyX), distY = abs(playerY - enemyY);
    int maxDist = (currentAttackType == AttackType::MELEE) ? 1 : 3;
    
    if (distX <= maxDist && distY <= maxDist) {
        int dmg = getDamage();
        enemy.takeDamage(dmg);
        std::cout << "Вы атаковали врага (" << getAttackTypeString() << ")! " << dmg << " урона!\n";
        return true;
    } else {
        std::cout << "Враг слишком далеко для " << getAttackTypeString() << "! ";
        std::cout << "Дистанция: " << std::max(distX, distY) << " клеток\n";
        return false;
    }
}