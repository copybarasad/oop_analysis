#include "SpellDirectDamage.h"
#include "Map.h"   
#include "Entity.h"
#include <iostream>
#include <cmath>

SpellDirectDamage::SpellDirectDamage(int damage, int radius)
    : Spell(damage, radius) {
    UpdateInfo();
}

void SpellDirectDamage::UseSpell(int target_x, int target_y, Map& map) {
    auto& objects = map.GetObjects();
    auto it = objects.find({ target_x, target_y });

    if (it != objects.end()) {
        Entity* target = it->second;
        target->TakeDamage(damage, map);
        std::cout << "Direct damage spell hit for " << damage << " damage!" << std::endl;
    }
    else {
        std::cout << "No target at specified coordinates!" << std::endl;
    }
}

bool SpellDirectDamage::CanCast(int caster_x, int caster_y, int target_x, int target_y, Map& map) {
    double distance = sqrt(pow(caster_x - target_x, 2) + pow(caster_y - target_y, 2));
    bool in_range = distance <= range;
    bool target_find = 1;
    bool valid_coords = (target_x >= 0 && target_x < map.GetHeight() &&
        target_y >= 0 && target_y < map.GetWeight());
    auto& objects = map.GetObjects();
    auto it = objects.find({ target_x, target_y });
    if (it == objects.end()) {
        target_find = 0;
    }
    return in_range && valid_coords && target_find;
}