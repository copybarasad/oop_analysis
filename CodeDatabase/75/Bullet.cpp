#include "Bullet.h"
#include <cmath>

Bullet::Bullet(int startX, int startY, Direction direction, int damage, Type type)
    : m_x(startX), m_y(startY), 
      m_visualX(startX * 48.0f), m_visualY(startY * 48.0f),
      m_direction(direction), m_damage(damage), m_active(true), m_type(type),
      m_moveTimer(0.0f)
{
    // Speeds: Higher = Slower
    switch(type) {
        case Type::ARROW: m_speedDelay = 0.05f; break; 
        case Type::MAGIC_MISSILE: m_speedDelay = 0.06f; break;
        case Type::FIREBALL: m_speedDelay = 0.07f; break;
        case Type::DARK_ORB: m_speedDelay = 0.09f; break; 
        case Type::BOSS_BLAST: m_speedDelay = 0.12f; break; 
        default: m_speedDelay = 0.08f; break;
    }
}

void Bullet::update(float dt) {
    if (!m_active) return;
    
    // Logic Move
    m_moveTimer += dt;
    while (m_moveTimer >= m_speedDelay) {
        m_moveTimer -= m_speedDelay;
        switch (m_direction) {
            case Direction::UP: m_y--; break;
            case Direction::DOWN: m_y++; break;
            case Direction::LEFT: m_x--; break;
            case Direction::RIGHT: m_x++; break;
        }
    }
    
    // Visual Move (Increased speed to catch up faster)
    float targetX = m_x * 48.0f;
    float targetY = m_y * 48.0f;
    float speed = 30.0f; // Faster catchup so it doesn't look like it hits air
    
    m_visualX += (targetX - m_visualX) * speed * dt;
    m_visualY += (targetY - m_visualY) * speed * dt;
}