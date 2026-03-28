#include "sacred_glow_spell.h"
#include "field.h"
#include "character.h"

SacredGlowSpell::SacredGlowSpell() 
    : Spell("Священный Свет", 4, SpellType::SACRED_GLOW) {}

SpellCastResult SacredGlowSpell::calculateCast(const Position& casterPos, const Position& targetPos) const {
    SpellCastResult result;
    for (int dx = 0; dx < 2; ++dx) {
        for (int dy = 0; dy < 2; ++dy) {
            Position areaPos = targetPos.translated(dx, dy);  
            result.affectedPositions.push_back(areaPos);
        }
    }
    result.damage = 20;
    result.isTrap = false;
    return result;  
}
bool SacredGlowSpell::canCast(const Position& casterPos, const Position& targetPos)const {
    return casterPos.manhattanDistance(targetPos) <= getRange();
} 