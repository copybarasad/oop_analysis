#ifndef GAMELB1_CREATURE_H
#define GAMELB1_CREATURE_H

#include "Entity.h"

class Creature : public Entity {
public:
    Creature(int health, int damage, int x, int y);

    bool isAlive() const;
    void takeDamage(int damage);
    int getHealth() const;
    virtual int getDamage() const;

protected:
    int health_;
    int damage_;
};

#endif //GAMELB1_CREATURE_H