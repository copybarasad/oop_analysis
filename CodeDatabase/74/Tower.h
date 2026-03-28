#ifndef TOWER_H
#define TOWER_H

#include "Types.h"
#include "Player.h"
#include "DamageSpell.h"
#include "Serialization.h"

class Tower {
private:
    Coords pos;
    int range;
    int skip;
    DamageSpell spell;

public:
    Tower(Coords pos, int range, int damage, int skip);

    Coords getCoords() const { return pos; }
    int getDamage() const { return spell.getDamage(); }
    int getRange() const { return range; }
    bool tryAttack(Player& player, int move);

    json serialize() const;
    void deserialize(const json& j);
};

#endif // TOWER_H