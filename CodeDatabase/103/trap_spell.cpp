#include "trap_spell.h"

TrapSpell::TrapSpell(int dmg, int cost, int r) : trapDamage(dmg), cost(cost), radius(r) {}

std::string TrapSpell::getName() const { return "Trap"; }
int TrapSpell::getCost() const { return cost; }
int TrapSpell::getRadius() const { return radius; }
int TrapSpell::getDamage() const { return trapDamage; }

void TrapSpell::upgrade() {
    trapDamage += 15;  
    cost += 5;
}

void TrapSpell::cast(GeneralManager& genMan, std::pair<int, int> target) {
    Field& field = genMan.getField();
    field.setTrapCell(target.first, target.second, trapDamage,  CellType::TRAP);
}