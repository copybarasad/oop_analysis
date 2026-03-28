#pragma once
#include "../../saving/Saving.h"
#include "../utils/Values.h"
#include "Entity.h"

class Enemy : public Entity{
public:
    Enemy();
    Enemy(int lifes);
    ~Enemy();

    bool is_dead();

    Saving::EnemyData serialize() const;
    void deserialize(const Saving::EnemyData& data);
};


