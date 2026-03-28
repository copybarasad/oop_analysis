#ifndef RPG_ALLY_H
#define RPG_ALLY_H

#include "Entity.h"

class Ally : public Entity {
public:
    Ally(unsigned int h, unsigned int d, unsigned int viewDiameter);

    explicit Ally(json& data);

    [[nodiscard]] std::unique_ptr<Entity> clone() const override;
};



#endif