#ifndef GAMELB1_ALLY_H
#define GAMELB1_ALLY_H

#include "Creature.h"

class Game;

class Ally : public Creature {
public:
    Ally(int health, int damage, int x, int y);


    void update(Game& game);
};

#endif //GAMELB1_ALLY_H