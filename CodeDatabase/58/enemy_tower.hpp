#pragma once

#include "entity.hpp"
#include "enemy.hpp"

#include "waterball.hpp"
#define BASE_HP_ENEMY_TOWER 5
#define BASE_DAMAGE_ENEMY_TOWER 5
#define BASE_RANGE_ENEMY_TOWER 3
#define BASE_COEF_ENEMY_TOWER 0.5


class EnemyTower : public Entity {
private:
    int elapsed_since_shot{0};
    WaterBall waterball{BASE_RANGE_ENEMY_TOWER, BASE_COEF_ENEMY_TOWER};

public:
    EnemyTower(int hp = BASE_HP_ENEMY_TOWER, int damage = BASE_DAMAGE_ENEMY_TOWER);
    EnemyTower(int hp, int damage, int elapsed_since_shot);
    EnemyTower(EnemyTower* enemy_tower);
    virtual ~EnemyTower() override = default;

    int get_elapsed_since_shot();
    void add_elapsed_since_shot();
    void set_elapsed_since_shot(int n);

    int get_range();
    WaterBall& get_waterball();

    virtual char get_symbol() override;
};