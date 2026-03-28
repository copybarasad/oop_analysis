#include "Tower.h"
#include "GameField.h"
#include <iostream>
#include <cmath>

Tower::Tower(int centerX, int centerY, int damage, int attackRange, int attackCooldownMs)
    : Character(centerX, centerY, TOWER_SYMBOL, "Tower", TOWER_BASE_HEALTH, 0),
      m_field(nullptr), m_damage(damage), m_attackRange(attackRange), m_attackCooldownMs(attackCooldownMs)
{
    generateTowerBlocks(centerX, centerY);
    m_lastAttackTime = std::chrono::steady_clock::now();
}

Tower::~Tower() {}
void Tower::setGameField(GameField* field) { m_field = field; }

void Tower::update(float dt) {
    Character::update(dt);
}

void Tower::generateTowerBlocks(int centerX, int centerY) {
    m_blocks.clear();
    m_blocks.emplace_back(centerX, centerY);
    m_blocks.emplace_back(centerX - 1, centerY - 1);
    m_blocks.emplace_back(centerX + 1, centerY - 1);
    m_blocks.emplace_back(centerX - 1, centerY + 1);
    m_blocks.emplace_back(centerX + 1, centerY + 1);
}

bool Tower::attackPlayer(int playerX, int playerY) {
    if (!canAttack()) return false;
    if (!isInRange(playerX, playerY)) return false;
    if (!hasLineOfSight(playerX, playerY)) return false;
    m_lastAttackTime = std::chrono::steady_clock::now();
    return true;
}

bool Tower::canAttack() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastAttackTime);
    return elapsed.count() >= m_attackCooldownMs;
}

bool Tower::isInRange(int targetX, int targetY) const {
    int distance = std::abs(targetX - m_x) + std::abs(targetY - m_y);
    return distance <= m_attackRange;
}

bool Tower::hasLineOfSight(int targetX, int targetY) const {
    if (!m_field) return false;
    return hasDirectLineOfSight(targetX, targetY);
}

bool Tower::hasDirectLineOfSight(int targetX, int targetY) const {
    if (!m_field) return false;
    int dx = targetX - m_x; int dy = targetY - m_y;
    int steps = std::max(std::abs(dx), std::abs(dy));
    if (steps == 0) return true; 
    float xInc = static_cast<float>(dx) / steps; float yInc = static_cast<float>(dy) / steps;
    float cX = m_x + 0.5f; float cY = m_y + 0.5f;
    for (int i = 1; i < steps; i++) {
        cX += xInc; cY += yInc;
        int checkX = static_cast<int>(cX); int checkY = static_cast<int>(cY);
        if ((checkX == m_x && checkY == m_y) || (checkX == targetX && checkY == targetY)) continue;
        bool isSelf = false; for(auto& b : m_blocks) { if(b.first == checkX && b.second == checkY) { isSelf = true; break; } } if (isSelf) continue;
        if (!m_field->isInBounds(checkX, checkY)) return false;
        Cell::Type t = m_field->getCell(checkX, checkY).getType();
        if (t == Cell::WALL || t == Cell::OBSTACLE) return false;
    }
    return true;
}
Bullet::Direction Tower::getDirectionToTarget(int targetX, int targetY) const {
    int dx = targetX - m_x; int dy = targetY - m_y;
    if (std::abs(dx) > std::abs(dy)) return (dx > 0) ? Bullet::Direction::RIGHT : Bullet::Direction::LEFT;
    return (dy > 0) ? Bullet::Direction::DOWN : Bullet::Direction::UP;
}