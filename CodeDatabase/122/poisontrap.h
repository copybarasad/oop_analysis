#ifndef POISONTRAP_H
#define POISONTRAP_H

#include "observer.h"
#include "constants.h"
#include "gameboard.h"
#include "enemy.h"

class PoisonTrap : public Observer
{
public:
    PoisonTrap(int trapDamage, std::vector<std::unique_ptr<Enemy>>& targets, int x, int y);

    bool handleEvent(int x, int y) override;
    bool isDestroyed() const override { return m_destroyed; }

private:
    int m_damage;
    std::vector<std::unique_ptr<Enemy>>& m_targets;
    int m_x, m_y;
    bool m_destroyed;
};

#endif // POISONTRAP_H
