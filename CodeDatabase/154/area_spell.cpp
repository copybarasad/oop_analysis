#include "area_spell.h"
#include "field.h"
#include "combat_system.h"

AreaDamageSpell::AreaDamageSpell(int damage, int range)
: damage_(damage), range_(range) {}

bool AreaDamageSpell::apply(int casterId, Field& field, int targetRow, int targetCol) {
    if (!field.inBounds(targetRow, targetCol)) return false;
    int cr, cc;
    if (!field.getPosition(casterId, cr, cc)) return false;
    int dist = field.manhattanDistance(cr, cc, targetRow, targetCol);
    Player* pl = field.getPlayerById(casterId);
    if (dist > range_) return false;
    
    int areaSize = 2;
    if (pl) areaSize += pl->consumeExtraArea();
    for (int dr = 0; dr < areaSize; ++dr) {
        for (int dc = 0; dc < areaSize; ++dc) {
            int r = targetRow + dr;
            int c = targetCol + dc;
            if (!field.inBounds(r,c)) continue;
            int oid = field.getOccupantIdAt(r,c);
            if (oid >= 0) CombatSystem::dealDamage(field, oid, damage_, casterId);
        }
    }
    return true;
}

int AreaDamageSpell::getDamage() const { return damage_; }
int AreaDamageSpell::getRange() const { return range_; }

std::string AreaDamageSpell::getName() const { return "AreaDamage"; }
