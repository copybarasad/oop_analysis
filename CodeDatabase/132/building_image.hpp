#pragma once

#include <vector>
#include "effect_image.hpp"

class BuildingImage {
public:
    BuildingImage(int hp, int cd, int tmr, std::vector<EffectImage> effect_img)
        : health_points(hp), cooldown(cd), timer(tmr), effect_img(effect_img) {}
    
    int health_points;
    std::vector<EffectImage> effect_img;
    int cooldown;
    int timer;
};