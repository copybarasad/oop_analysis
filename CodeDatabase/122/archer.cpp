#include "archer.h"

Archer::Archer(int health, int xPos, int yPos, int meleeDamage)
    : Entity(health, xPos, yPos, meleeDamage) {}

void Archer::move(const Cell &cell, int dx, int dy)
{
    m_xPos += dx;
    m_yPos += dy;

    if (cell.getType() == CellType::SLOWED) {
        m_effect = DELAYED;
    }
}

int Archer::attack(Entity &other)
{
    bool isMeleeRange = isInRange(other, 1);

    if (isMeleeRange)
    {
        other.takeDamage(m_meleeDamage);
        return m_meleeDamage;
    }
    return 0;
}

void Archer::takeDamage(int damage)
{
    m_health -= damage;
    if (m_health <= 0)
        kill();
}

std::pair<int, int> Archer::calculateMovement(int chaserX, int chaserY, int targetX, int targetY)
{
    std::uniform_int_distribution<> die{0, 100};

    if (die(MyRandom::Mersenne) < 30) {
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
