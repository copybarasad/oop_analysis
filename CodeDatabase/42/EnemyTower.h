#pragma once
#include "Position.h"

class SaveManager;

class EnemyTower {
private:
    Position position;
    int health;
    int maxHealth;
    int damage;
    int attackRange;
    int turnsUntilNextAttack;
    
    void takeDamageInternal(int dmg);
    void resetAttackCooldownInternal();
    void decreaseCooldownInternal();
    
    void setCooldownInternal(int cooldown);
    
    friend class SaveManager;
    
public:
    EnemyTower(const Position& pos, int towerHealth, int towerDamage, int range);
    
    Position getPosition() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getAttackRange() const;
    bool isAlive() const;
    bool canAttackThisTurn() const;
    
    void takeDamage(int dmg);
    void decreaseCooldown();
    void resetAttackCooldown() { resetAttackCooldownInternal(); }
    
    int getCurrentCooldown() const;
    
    EnemyTower(const EnemyTower&) = delete;
    EnemyTower& operator=(const EnemyTower&) = delete;
    
    EnemyTower(EnemyTower&&) noexcept;
    EnemyTower& operator=(EnemyTower&&) noexcept;
    ~EnemyTower() = default;
};