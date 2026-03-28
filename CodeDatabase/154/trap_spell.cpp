#include "trap_spell.h"
#include "field.h"
#include "trap.h"

TrapSpell::TrapSpell(int damage, int range): damage_(damage), range_(range) {}

bool TrapSpell::apply(int casterId, Field& field, int targetRow, int targetCol) {
    if (!field.inBounds(targetRow, targetCol)) return false;
    int cr, cc;
    if (!field.getPosition(casterId, cr, cc)) return false;
    int dist = field.manhattanDistance(cr, cc, targetRow, targetCol);
    Player* pl = field.getPlayerById(casterId);
    int effDamage = damage_;
    if (pl) effDamage += pl->consumeExtraTrapDamage();
    if (dist > range_) return false;
    if (!field.cellIsEmpty(targetRow, targetCol)) return false;
    int nid = field.generateEntityId();
    Trap* trap = new Trap(nid, effDamage);
    field.placeEntity(trap, targetRow, targetCol);
    return true;
}

std::string TrapSpell::getName() const { return "TrapSpell"; }
