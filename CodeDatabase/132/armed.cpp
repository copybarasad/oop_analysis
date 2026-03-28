#include "armed.hpp"

Armed::Armed(std::vector<Weapon> weapons): 
    weapons(weapons), weapon_index(0) 
{};

Armed::Armed(std::vector<Weapon> weapons, int weapon_index): 
    weapons(weapons), weapon_index(weapon_index) 
{};

void Armed::change_weapon() {
    weapon_index = (weapon_index + 1) % weapons.size();
}

Weapon Armed::get_current_weapon() const {
    return weapons[weapon_index];
}