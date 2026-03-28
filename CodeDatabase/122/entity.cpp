#include "entity.h"

Entity::Entity(int health, int xPos, int yPos, int meleeDamage)
    : m_health(health), m_xPos(xPos), m_yPos(yPos), m_meleeDamage(meleeDamage), m_effect(NO_EFFECT), m_lifeStatus(ALIVE) {}

bool Entity::isAlive() const
{
    return m_lifeStatus == ALIVE;
}

bool Entity::isInRange(Entity& other, int range) const
{
    return std::abs(m_xPos - other.getXPos()) <= range &&
           std::abs(m_yPos - other.getYPos()) <= range;
}

void Entity::kill()
{
    m_health = 0;
    m_lifeStatus = DEAD;
}

void Entity::addHealth(int health)
{
    m_health += health;
    m_lifeStatus = ALIVE;
}

std::vector<std::pair<int, int>> Entity::directions = {
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};
