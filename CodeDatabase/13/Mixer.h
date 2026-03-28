#ifndef GAME_MIXER_H
#define GAME_MIXER_H

#include "../basics/Weapon.h"

class Mixer : public Weapon {
public:
    explicit Mixer(Entity *owner) : Weapon(owner, "Mixer", 1, 3) {
    };
};
#endif
