#ifndef GAME_PAN_H
#define GAME_PAN_H

#include "../basics/Weapon.h"

class Pan : public Weapon {
public:
    explicit Pan(Entity *owner) : Weapon(owner, "Pan", 2, 1) {
    };
};

#endif
