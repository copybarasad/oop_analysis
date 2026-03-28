#include "Ally.h"
#include "GameField.h"
#include "Enemy.h"
#include "Game.h"
#include <cmath>
#include <iostream>

Ally::Ally(int startX, int startY, const std::string& name, int maxHealth, int damage)
    : Character(startX, startY, 'A', name, maxHealth, 0),
      m_field(nullptr), m_damage(damage), m_moveTimer(0), m_attackTimer(0)
{}

Ally::~Ally() {}
void Ally::setGameField(GameField* field) { m_field = field; }

void Ally::update(float dt) {
    Character::update(dt);
    updateStatusEffects();
    m_moveTimer += dt;
    m_attackTimer += dt;
}

void Ally::updateAI(const std::vector<Enemy*>& enemies, Game* game) {
    Enemy* target = nullptr;
    float minDist = 999.0f;
    int ax = getX(); int ay = getY();
    
    for (auto* e : enemies) {
        if (!e->isAlive()) continue;
        float d = std::abs(e->getX() - ax) + std::abs(e->getY() - ay);
        if (d < minDist && d < 15.0f) {
            minDist = d;
            target = e;
        }
    }
    
    if (target) {
        // Combat Logic
        if (minDist <= 1.5f) { // Adjacent
            if (m_attackTimer > 1.0f) {
                target->getDamaged(m_damage);
                game->addVisualEvent(VisualEvent::SLASH, ax, ay, target->getX(), target->getY(), sf::Color::Green);
                m_attackTimer = 0;
            }
        } else {
            if (m_moveTimer > 0.4f) {
                moveTowards(target->getX(), target->getY());
                m_moveTimer = 0;
            }
        }
    } else {
        Player* p = game->getPlayer();
        if(p) {
            float distToP = std::abs(p->getX() - ax) + std::abs(p->getY() - ay);
            if (distToP > 3.0f) {
                if (m_moveTimer > 0.4f) {
                    moveTowards(p->getX(), p->getY());
                    m_moveTimer = 0;
                }
            }
        }
    }
}

void Ally::moveTowards(int targetX, int targetY) {
    int dx = targetX - m_x; int dy = targetY - m_y;
    int nextX = m_x; int nextY = m_y;
    
    if (std::abs(dx) > std::abs(dy)) nextX += (dx > 0 ? 1 : -1);
    else nextY += (dy > 0 ? 1 : -1);
    
    if (isValidMove(nextX, nextY)) setPosition(nextX, nextY);
    else {
        if (std::abs(dx) > std::abs(dy)) nextY += (dy > 0 ? 1 : -1);
        else nextX += (dx > 0 ? 1 : -1);
        if (isValidMove(nextX, nextY)) setPosition(nextX, nextY);
    }
}

bool Ally::isValidMove(int x, int y) const {
    if (!m_field || !m_field->isInBounds(x, y)) return false;
    Cell::Type t = m_field->getCell(x, y).getType();
    return t != Cell::WALL && t != Cell::OBSTACLE;
}