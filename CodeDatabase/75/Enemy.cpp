#include "Enemy.h"
#include "GameField.h"
#include "Bullet.h"
#include "Cell.h" 
#include <cmath>
#include <iostream>
#include <algorithm>

std::vector<Enemy*> Enemy::s_spawnQueue;

Enemy::Enemy(int x, int y, Type type, const std::string& name, int maxHealth, int damage)
    : Character(x, y, 'E', name, maxHealth, damage), 
      m_type(type), m_damage(damage), m_field(nullptr), m_attackTimer(0), m_moveTimer(0)
{}

Enemy::~Enemy() { for(auto* b : m_bullets) delete b; }
void Enemy::setGameField(GameField* field) { m_field = field; }

void Enemy::spawn(int x, int y, Type type, int difficulty) {
    // Reduced HP slightly for better balance
    int hp = 40 + difficulty * 8; 
    int dmg = 5 + difficulty;
    std::string name = "Minion";
    
    if (type == Type::RANGED) { hp = 25 + difficulty * 4; name = "Archer"; dmg = 8 + difficulty; } 
    else if (type == Type::FAST) { hp = 30 + difficulty * 6; name = "Assassin"; dmg = 12 + difficulty; } 
    else if (type == Type::TANK) { hp = 80 + difficulty * 15; name = "Golem"; dmg = 15 + difficulty; } 
    else if (type == Type::MAGE) { hp = 35 + difficulty * 4; name = "Necromancer"; dmg = 10 + difficulty * 2; }
    
    s_spawnQueue.push_back(new Enemy(x, y, type, name, hp, dmg));
}

std::string Enemy::getTypeName() const {
    switch(m_type) {
        case Type::MELEE: return "ORC"; case Type::RANGED: return "ARCHER"; case Type::FAST: return "ASSASSIN"; case Type::TANK: return "GOLEM"; case Type::MAGE: return "NECROMANCER"; case Type::BOSS: return "DEMON LORD"; default: return "ENEMY";
    }
}

void Enemy::update(float dt) {
    Character::update(dt);
    updateStatusEffects();
    m_attackTimer += dt;
    m_moveTimer += dt;
    
    // Pass DT to bullets
    for(auto* b : m_bullets) { if (b->isActive()) b->update(dt); }
    
    auto it = std::remove_if(m_bullets.begin(), m_bullets.end(), [](Bullet* b){ if (!b->isActive()) { delete b; return true; } return false; });
    m_bullets.erase(it, m_bullets.end());
}

void Enemy::moveTowards(int px, int py) {
    float moveDelay = 0.5f; 
    if (m_type == Type::FAST) moveDelay = 0.3f;
    if (m_type == Type::TANK) moveDelay = 0.8f;
    if (m_type == Type::BOSS) moveDelay = 1.0f; 
    
    if (m_moveTimer < moveDelay) return;
    
    int dx = px - m_x; int dy = py - m_y;
    int dist = std::abs(dx) + std::abs(dy);
    int stopRange = 1; if (m_type == Type::RANGED || m_type == Type::MAGE) stopRange = 5;
    if (dist <= stopRange) return;

    int nextX = m_x; int nextY = m_y;
    if (std::abs(dx) > std::abs(dy)) { nextX += (dx > 0 ? 1 : -1); } else { nextY += (dy > 0 ? 1 : -1); }
    
    if (m_field && m_field->isInBounds(nextX, nextY)) {
        Cell::Type t = m_field->getCell(nextX, nextY).getType();
        if (t == Cell::EMPTY) { setPosition(nextX, nextY); m_moveTimer = 0; }
    }
}

void Enemy::shootAtPlayer(int px, int py) {
    if (m_attackTimer < 2.0f) return;
    float dx = px - m_x; float dy = py - m_y;
    Bullet::Direction dir = Bullet::Direction::DOWN;
    if (std::abs(dx) > std::abs(dy)) { dir = (dx > 0) ? Bullet::Direction::RIGHT : Bullet::Direction::LEFT; } 
    else { dir = (dy > 0) ? Bullet::Direction::DOWN : Bullet::Direction::UP; }
    
    Bullet::Type bType = Bullet::Type::ARROW;
    if (m_type == Type::MAGE || m_type == Type::BOSS) bType = Bullet::Type::DARK_ORB;
    
    m_bullets.push_back(new Bullet(m_x, m_y, dir, m_damage, bType));
    m_attackTimer = 0;
}