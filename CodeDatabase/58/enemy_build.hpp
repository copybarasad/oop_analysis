#pragma once

#include "entity.hpp"


#define BASE_HP_BUILD 20
#define BASE_DAMAGE_BUILD 0
#define BASE_REQUIRED_QUANTITY 2

class EnemyBuild : public Entity{
private:
    int required_quantity{BASE_REQUIRED_QUANTITY};
public:
    EnemyBuild(int required_quantity = BASE_REQUIRED_QUANTITY, int hp = BASE_HP_BUILD, int damage = BASE_DAMAGE_BUILD);
    EnemyBuild(EnemyBuild* enemy_build);
    virtual ~EnemyBuild() override = default;

    int get_required_quantity();

    virtual char get_symbol() override;

};