#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "GameObject.h"

class EnemyTower : public GameObject {
private:
    int damage;
    int attackRadius;
    bool canAttack;
    int weakenedDamage;    // Ослабленный урон для "заклинания башни"
    int attackCooldown;    // Перезарядка между атаками
    
public:
    EnemyTower(int health = 80, int damage = 15, int attackRadius = 4);
    
    int getDamage() const;
    int getAttackRadius() const;
    bool canAttackNow() const;
    void resetAttack();
    
    // Методы для ослабленного урона (критерий 6) ДОЛЖНО БЫТЬ ЗАКЛИНАНИЕМ
    int getWeakenedDamage() const;
    
    // Методы для системы перезарядки (критерий 6)
    void setCooldown(int cooldown);
    void updateCooldown();
    bool isOnCooldown() const;
    int getCooldown() const;
    
    std::string getType() const override;
};

#endif
