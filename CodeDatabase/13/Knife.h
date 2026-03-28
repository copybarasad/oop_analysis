#ifndef GAME_KNIFE_H
#define GAME_KNIFE_H

#include "../basics/Weapon.h"

class Knife : public Weapon {
public:
    explicit Knife(Entity *owner) : Weapon(owner, "Knife", 2, 1) {
    };
};

#endif