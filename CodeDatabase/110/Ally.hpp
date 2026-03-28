#ifndef ALLY_H
#define ALLY_H

#include "../Entity/Entity.hpp"

class Ally : public Entity {
    public:
    Ally (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, unsigned int damageValue);
    
    unsigned int calculateDamage () const override;
};

#endif