#ifndef TRAP_H
#define TRAP_H

#include "entity.h"

class Trap : public Entity {
public:
    Trap(int id, int damage);
    EntityType getType() const override;
    int getDamage() const;
    Trap* asTrap();
private:
    int damage_;
};

#endif
