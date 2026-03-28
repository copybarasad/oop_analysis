#pragma once
#include <string>

enum class SpellType { directDamage, areaDamage, trap, summAlly, empower, none };

std::string spellTypetoString(SpellType type);
SpellType stringToSpellType(const std::string& s);