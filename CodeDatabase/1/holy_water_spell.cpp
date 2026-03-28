#include "holy_water_spell.h"
#include "field.h"
#include "character.h"

HolyWaterSpell::HolyWaterSpell() 
    : Spell("Святая Вода", 3, SpellType::HOLY_WATER) {}

SpellCastResult HolyWaterSpell::calculateCast(const Position& casterPos, const Position& targetPos) const {
    SpellCastResult result;
    result.affectedPositions.push_back(targetPos);
    result.damage = 30;
    result.isTrap = true;
    return result;  
}
bool HolyWaterSpell::canCast(const Position& casterPos, const Position& targetPos)const {
    return casterPos.manhattanDistance(targetPos) <= getRange();
} 