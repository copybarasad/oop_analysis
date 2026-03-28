#ifndef SPELL_CAST_H
#define SPELL_CAST_H

#include "Spell.h"
#include "player.h"
#include "field.h"
#include "directed_spell.h"
#include "area_spell.h"
#include "trap_spell.h"
#include <cmath>

class SpellCast {
private:
    static bool castDirectSpell(Spell& spell, Player& caster, Field& field, int target_x, int target_y);
    static bool castAreaSpell(Spell& spell, Player& caster, Field& field, int target_x, int target_y);
    static bool castTrapSpell(Spell& spell, Player& caster, Field& field, int target_x, int target_y);

    static bool castDirectSpellFromTower(Spell& spell, Field& field, int target_x, int target_y);

    
    static bool checkMana(Player& caster, Spell& spell);
    static bool checkRange(Player& caster, int target_x, int target_y, int range);
    static bool isValidTarget(Field& field, int x, int y);

public:
    static bool cast(Spell& spell, Player& caster, Field& field, int target_x, int target_y);
    static bool castFromTower(Spell& spell, Field& field, int target_x, int target_y);

};

#endif