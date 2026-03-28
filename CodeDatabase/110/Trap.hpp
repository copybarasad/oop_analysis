#ifndef TRAP_H
#define TRAP_H

#include "../../Entity/Entity.hpp"

class Trap : public Entity {
    public:
    Trap (size_t id, unsigned int damageValue, unsigned int maxHealthValue=0, unsigned int currentHealthValue=0);

    unsigned int calculateDamage () const override;
};

#endif