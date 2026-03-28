#include "light_beam_spell.h"
#include "field.h"
#include "character.h"

LightBeamSpell::LightBeamSpell() 
    : Spell("Луч Света", 5, SpellType::LIGHT_BEAM) {}

SpellCastResult LightBeamSpell::calculateCast(const Position& casterPos, const Position& targetPos) const {
    SpellCastResult result;
    result.affectedPositions.push_back(targetPos);
    result.damage = 25;
    result.isTrap = false;
    return result;  
}
bool LightBeamSpell::canCast(const Position& casterPos, const Position& targetPos)const {
    return casterPos.manhattanDistance(targetPos) <= getRange();
} 


