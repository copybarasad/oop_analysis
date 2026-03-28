#ifndef ALLY_H
#define ALLY_H

#include "Entity.h"

class Ally : public Entity {
public:
    Ally(int start_x, int start_y, int health, int damage);
    void update(Game& game) override;
};

#endif