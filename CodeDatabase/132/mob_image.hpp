#pragma once

#include <vector>
#include "effect_image.hpp"

class MobImage {
public:
    MobImage(int hp, bool movable, std::vector<EffectImage> effect_img)
        : health_points(hp), can_move(movable), effect_img(effect_img) {}
    
    int health_points;
    std::vector<EffectImage> effect_img;
    bool can_move;
};