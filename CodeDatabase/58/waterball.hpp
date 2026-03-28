#pragma once

#include "i_direct_damage.hpp"
#include <map>

#define BASE_RANGE_WATERBALL 3
#define BASE_COEF_WATERBALL 0.75

class WaterBall : public IDirectDamage{
private:
    std::string name = "WaterBall";
    float coef = 0.75;
    int range{BASE_RANGE_WATERBALL};

public:
    WaterBall(int range = BASE_RANGE_WATERBALL, float coef = BASE_COEF_WATERBALL);
    virtual ~WaterBall() = default;

    virtual std::string get_name() override;
    virtual bool cast(World* world) override;

    Cell* get_enemy_to_attack(World* world);
    int get_range();
    float get_coef();

    virtual int get_damage(Entity& entity) override;
};