#include "spell.h"
#include "game_controller.h"

Spell::Spell(const std::string& spell_name, const std::string& spell_desc, int spell_range)
    : name(spell_name),
      description(spell_desc),
      range(spell_range) {}