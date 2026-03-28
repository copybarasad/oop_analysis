#include "SpellTrap.h"
#include "Map.h"   
#include "Entity.h"
#include <cmath>
#include <iostream>

SpellTrap::SpellTrap(int damage, int radius)
    : Spell(damage, radius) {
    UpdateInfo(); 
}

void SpellTrap::UseSpell(int target_x, int target_y, Map& map) {
    if (map.GetVal(target_x, target_y) == '0') {
        map.SetVal(target_x, target_y, '*', nullptr);
        std::cout << "Trap placed at coordinates (" << target_x << ", " << target_y << ")!" << std::endl;
    }
    else {
        std::cout << "Cannot place trap here - cell is occupied!" << std::endl;
    }
}

bool SpellTrap::CanCast(int caster_x, int caster_y, int target_x, int target_y, Map& map) {
    double distance = sqrt(pow(caster_x - target_x, 2) + pow(caster_y - target_y, 2));
    bool in_range = distance <= range;

    bool valid_coords = (target_x >= 0 && target_x < map.GetHeight() &&
        target_y >= 0 && target_y < map.GetWeight());
    bool cell_free = valid_coords && (map.GetVal(target_x, target_y) == '0');

    return in_range && cell_free;
}