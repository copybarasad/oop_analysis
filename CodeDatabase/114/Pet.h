#ifndef PET_H
#define PET_H

#include "Entity.h"

class Pet : public Entity {
public:
    Pet() : Entity(1, 1) {}
    int getDamage() const override { return 0; }
    
    std::string getEntityName() const override { return "Союзник"; }
};

#endif



