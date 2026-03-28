#ifndef UNTITLED_ALLY_H
#define UNTITLED_ALLY_H

#include "Enemy.h"

class Ally : public Enemy {
public:
    Ally(int hp = 3, int dmg = 1);
    ~Ally() override = default;

    std::shared_ptr<Entity> clone() const override;
};


#endif //UNTITLED_ALLY_H
