#include "enemy.h"

Enemy::Enemy(int health, int xPos, int yPos, int meleeDamage)
    : Entity(health, xPos, yPos, meleeDamage), m_pathFlag(NO_CHANGE), m_attackResult(MISSED) {}

void Enemy::trigger()
{
    m_pathFlag = CHANGE_NEEDED;
}

void Enemy::move(const Cell &cell, int dx, int dy)
{
    m_xPos += dx;
    m_yPos += dy;

    if (cell.getType() == CellType::SLOWED) {
        m_effect = DELAYED;
    }
}

int Enemy::attack(Entity& other)
{
    bool isMeleeRange = isInRange(other, 1);

    if (isMeleeRange)
    {
        other.takeDamage(m_meleeDamage);
        m_attackResult = HIT;
        return m_meleeDamage;
    }
    else
    {
        m_attackResult = MISSED;
        return 0;
    }
}

void Enemy::takeDamage(int damage)
{
    m_pathFlag = CHANGE_NEEDED;
    m_health -= damage;
    if (m_health <= 0)
        kill();
}

std::pair<int, int> Enemy::calculateMovement(int chaserX, int chaserY, int targetX, int targetY)
{
    std::uniform_int_distribution<> die{0, 100};

    if (m_pathFlag == CHANGE_NEEDED) {
        int randomIndex = die(MyRandom::Mersenne) % directions.size();
        m_pathFlag = NO_CHANGE;
        return directions[randomIndex];
    }

    if ((m_attackResult == MISSED) && die(MyRandom::Mersenne) < 20) {
        int randomIndex = die(MyRandom::Mersenne) % directions.size();
        return directions[randomIndex];
    }

    std::pair<int, int> bestMove = {0, 0};
    int minDistance = std::abs(targetX - chaserX) + std::abs(targetY - chaserY);

    for (const auto& [dx, dy] : directions)
    {
        int newX = chaserX + dx;
        int newY = chaserY + dy;

        int newDistance = std::abs(targetX - newX) + std::abs(targetY - newY);

        if (newDistance < minDistance)
        {
            minDistance = newDistance;
            bestMove = {dx, dy};
        }
    }
    return bestMove;
}
