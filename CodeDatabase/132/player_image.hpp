#pragma once

#include <vector>
#include "effect_image.hpp"
#include "hand_image.hpp"

class PlayerImage {
public:
    PlayerImage(int hp, bool can_move, int weapon_idx, HandImage hand, std::vector<EffectImage> effect_img)
        : health_points(hp), can_move(can_move), weapon_index(weapon_idx), hand_image(hand), effect_img(effect_img) {}
    
    int health_points;
    std::vector<EffectImage> effect_img;
    bool can_move;
    int weapon_index;
    HandImage hand_image;
};