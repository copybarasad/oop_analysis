#ifndef INCLUDE_ENEMY
#define INCLUDE_ENEMY

#include "entity.h"

class Enemy : public Entity {
private:
    int damage_;

public:
    Enemy();
    Enemy(int max_health, int damage);

    int damage() const;
};

#endif