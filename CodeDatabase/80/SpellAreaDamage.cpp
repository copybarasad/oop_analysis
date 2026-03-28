#include "SpellAreaDamage.h"
#include "Map.h"
#include "Entity.h"
#include <cmath>
#include <iostream>

SpellAreaDamage::SpellAreaDamage(int damage, int radius)
    : Spell(damage, radius), area_size(2) {
    UpdateInfo(); 
}

void SpellAreaDamage::UseSpell(int target_x, int target_y, Map& map) {
    int targets_hit = 0;

    for (int dx = -area_size; dx < area_size; dx++) {
        for (int dy = -area_size; dy < area_size; dy++) {
            int current_x = target_x + dx;
            int current_y = target_y + dy;

            if (current_x >= 0 && current_x < map.GetHeight() &&
                current_y >= 0 && current_y < map.GetWeight()) {

                char target_type = map.GetVal(current_x, current_y);
                if (target_type == 'e' || target_type == 'T' || target_type == 'B') {
                    auto& objects = map.GetObjects();
                    auto it = objects.find({ current_x, current_y });
                    if (it != objects.end()) {
                        Entity* target = it->second;
                        target->TakeDamage(damage, map);
                        targets_hit++;
                    }
                }
            }
        }
    }

    std::cout << "Area damage spell hit " << targets_hit << " targets!" << std::endl;
}

bool SpellAreaDamage::CanCast(int caster_x, int caster_y, int target_x, int target_y, Map& map) {
    double distance = sqrt(pow(caster_x - target_x, 2) + pow(caster_y - target_y, 2));
    bool in_range = distance <= range;

    bool valid_coords = (target_x >= 0 && target_x < map.GetHeight() &&
        target_y >= 0 && target_y < map.GetWeight());

    return in_range && valid_coords;
}