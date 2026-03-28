#ifndef GAMELB1_TRAP_H
#define GAMELB1_TRAP_H

#include "Entity.h"

class Trap : public Entity {
public:
    Trap(int damage, int x, int y);
    int getDamage() const;

private:
    int damage_;
};

#endif //GAMELB1_TRAP_H