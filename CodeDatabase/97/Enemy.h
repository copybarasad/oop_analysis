
#ifndef UNTITLED_ENEMY_H
#define UNTITLED_ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(int hp = 6, int dmg = 1);
    ~Enemy() override = default;

    std::shared_ptr<Entity> clone() const override;
};


#endif //UNTITLED_ENEMY_H
