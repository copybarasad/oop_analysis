#include "SpellBase.h"
#include "Constants.h"  

using namespace GameConstants;
    
SpellBase::SpellBase(const std::string& spell_name, int spell_level, int base_damage)
    : name(spell_name), level(spell_level), damage(base_damage) {}

std::string SpellBase::getName() const  { return name; }
int SpellBase::getLevel() const  { return level; }
int SpellBase::getDamage() const { return damage; }

void SpellBase::setLevel(int new_level) { 
    if (new_level >= 1 && new_level <= MAX_SPELL_LEVEL) {
        level = new_level; 
    }
}

void SpellBase::setDamage(int new_damage) { damage = new_damage; }