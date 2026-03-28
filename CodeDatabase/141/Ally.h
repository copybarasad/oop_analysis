#ifndef ALLY_H
#define ALLY_H

#include "Character.h"

class Game;

class Ally : public Character {
private:
    int damage;

public:
    Ally(int startX, int startY, int allyHealth, int allyDamage, int maxX, int maxY);

    int getDamage() const override;

    void move(Game &game);
};

#endif
