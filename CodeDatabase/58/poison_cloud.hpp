#pragma once

#include "i_area_damage.hpp"
#define BASE_AREA_POISON 2

class PoisonCloud : public IAreaDamage{
private:
    std::string name = "PoisonCloud";
    int area = BASE_AREA_POISON;
    float coef = 0.5;

public:
    virtual ~PoisonCloud() = default;

    float get_coef();
    int get_area();

    virtual std::string get_name() override;
    virtual bool cast(World* world) override;

    virtual int get_damage(Entity& entity) override;
};