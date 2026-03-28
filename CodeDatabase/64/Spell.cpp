#include "Spell.h"

Spell::Spell(const std::string& name, const std::string& description, 
             int range, SpellType type)
    : name(name), description(description), range(range), type(type) {}

std::string Spell::getName() const { return name; }
std::string Spell::getDescription() const { return description; }
int Spell::getRange() const { return range; }
SpellType Spell::getType() const { return type; }