#ifndef ALLY_H
#define ALLY_H

#include "entity.h"
#include <utility>

class Field;

class Ally : public Entity {
public:
    Ally(int id, int health, int damage);
    EntityType getType() const override;
    int getDamage() const override;
    Ally* asAlly() override { return this; }
    std::pair<int,int> chooseStep(const Field& field, int allyRow, int allyCol) const;

private:
    int damage_;
};

#endif