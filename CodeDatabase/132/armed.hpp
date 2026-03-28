#pragma once

#include "IArmed.hpp"
#include <vector>

class Armed: public IArmed {
protected:
    std::vector<Weapon> weapons;
    int weapon_index;
public:
    Armed(std::vector<Weapon> weapons);
    Armed(std::vector<Weapon> weapons, int weapon_index);
    void change_weapon() override;
    Weapon get_current_weapon() const override;
};