#include "poisontrap.h"

PoisonTrap::PoisonTrap(int trapDamage, std::vector<std::unique_ptr<Enemy>>& targets, int x, int y)
    :
    m_damage(trapDamage),
    m_targets(targets),
    m_x(x), m_y(y),
    m_destroyed(false) {}

bool PoisonTrap::handleEvent(int x, int y)
{
    if (m_x != x || m_y != y) return false;

    for (auto& target : m_targets) {
        if (target->getXPos() == m_x && target->getYPos() == m_y)
        {
            target->takeDamage(m_damage);
            m_destroyed = true;
            return true;
        }
    }
    return false;
}
