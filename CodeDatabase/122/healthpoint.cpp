#include "healthpoint.h"

HealthPoint::HealthPoint(std::vector<std::unique_ptr<Player>>& players, int x, int y)
    :
    m_healAmount(BoardConfig::cellHealAmount),
    m_players(players),
    m_x(x), m_y(y),
    m_destroyed(false)
{}

bool HealthPoint::handleEvent(int x, int y)
{
    if (m_x != x || m_y != y) return false;

    for (auto& player : m_players) {
        if (player->getXPos() == m_x && player->getYPos() == m_y)
        {
            player->addHealth(m_healAmount);
            m_destroyed = true;
            return true;
        }
    }
    return false;
}
