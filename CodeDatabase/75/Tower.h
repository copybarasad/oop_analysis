#pragma once
#include "Character.h"
#include "Bullet.h"
#include <chrono>
#include <vector>

class GameField;

class Tower : public Character {
public:
    Tower(int centerX, int centerY, int damage, int attackRange, int attackCooldownMs);
    ~Tower();
    
    void update(float dt) override; 
    void setGameField(GameField* field);
    bool attackPlayer(int playerX, int playerY);
    bool canAttack() const;
    bool isInRange(int targetX, int targetY) const;
    bool hasLineOfSight(int targetX, int targetY) const;
    int getDamage() const { return m_damage; }
    int getAttackRange() const { return m_attackRange; }
    const std::vector<std::pair<int, int>>& getBlocks() const { return m_blocks; }
    
private:
    static constexpr int TOWER_BASE_HEALTH = 1000;
    static constexpr char TOWER_SYMBOL = '&';
    static constexpr int TOWER_CROSS_SIZE = 3;
    
    GameField* m_field;
    std::vector<std::pair<int, int>> m_blocks;
    int m_damage;
    int m_attackRange;
    int m_attackCooldownMs;
    std::chrono::steady_clock::time_point m_lastAttackTime;
    void generateTowerBlocks(int centerX, int centerY);
    Bullet::Direction getDirectionToTarget(int targetX, int targetY) const;
    bool hasDirectLineOfSight(int targetX, int targetY) const;
};