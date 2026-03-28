#include "AreaSpell.h"

AreaSpell::AreaSpell(const std::string& name, int damage, int range, int areaSize)
    : Spell(name, "Deals " + std::to_string(damage) + " damage in " + 
            std::to_string(areaSize) + "x" + std::to_string(areaSize) + " area", 
            range, SpellType::AREA_EFFECT), damage(damage), areaSize(areaSize) {}