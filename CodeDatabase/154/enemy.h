#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include <utility>

class Enemy : public Entity {
public:
    Enemy(int id, int health, int damage);
    EntityType getType() const override;
    int getDamage() const override;
    std::pair<int,int> chooseStep(int pr, int pc, int er, int ec) const;
    Enemy* asEnemy() override;
private:
    int damage_;
};

#endif
