#include "TrapSpell.h"
#include "Field.h"

TrapSpell::TrapSpell(int damage, int mult)
    : baseDamage_(damage), damageMultiplier_(mult) {}

std::string TrapSpell::getName() const {
    return "Trap";
}

bool TrapSpell::apply(Field& field, const Player& player, int targetX, int targetY) {
    field.placeTrap(targetX, targetY);
    return true;
}