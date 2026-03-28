#include "direct_damage_spell.h"
#include "field.h"
#include "combat_system.h"

DirectDamageSpell::DirectDamageSpell(int damage, int range)
: damage_(damage), range_(range) {}

bool DirectDamageSpell::apply(int casterId, Field& field, int targetRow, int targetCol) {
    if (!field.inBounds(targetRow, targetCol)) return false;
    int cr, cc;
    if (!field.getPosition(casterId, cr, cc)) return false;
    int dist = field.manhattanDistance(cr, cc, targetRow, targetCol);
    Player* pl = field.getPlayerById(casterId);
    int effRange = range_;
    if (pl) effRange += pl->consumeExtraRange();
    if (dist > effRange) return false;
    int oid = field.getOccupantIdAt(targetRow, targetCol);
    if (oid < 0) return false;
    if (!field.isEnemy(oid) && !field.isBuilding(oid)) return false;
    CombatSystem::dealDamage(field, oid, damage_, casterId);
    return true;
}

int DirectDamageSpell::getDamage() const { return damage_; }
int DirectDamageSpell::getRange() const { return range_; }

std::string DirectDamageSpell::getName() const { return "DirectDamage"; }
