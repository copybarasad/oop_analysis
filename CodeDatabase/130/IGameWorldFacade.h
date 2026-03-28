#ifndef IGAMEWORLDFACADE_H
#define IGAMEWORLDFACADE_H

#include "Position.h"

class IGameWorldFacade {
public:
    virtual ~IGameWorldFacade() = default;

    virtual bool dealDamageToEnemy(Pos pos, int damage) = 0;
    virtual bool dealDamageToBuilding(Pos pos, int damage) = 0;
    virtual bool dealDamageToTower(Pos pos, int damage) = 0;

    virtual bool placeTrap(Pos pos, int damage) = 0;
    virtual bool summonAlly(Pos near, int count) = 0;

    virtual bool isInRange(Pos from, Pos to, int range) const = 0;
    virtual Pos getPlayerPosition() const = 0;
    virtual bool isValidPosition(Pos pos) const = 0;
};

#endif

