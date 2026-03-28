#ifndef HEALTHPOINT_H
#define HEALTHPOINT_H

#include "observer.h"
#include "player.h"

class HealthPoint : public Observer
{
public:
    HealthPoint(std::vector<std::unique_ptr<Player>>& players, int x, int y);

    bool handleEvent(int x, int y) override;
    bool isDestroyed() const override { return m_destroyed; }

private:
    int m_healAmount;
    std::vector<std::unique_ptr<Player>>& m_players;
    int m_x, m_y;
    bool m_destroyed;
};

#endif // HEALTHPOINT_H
