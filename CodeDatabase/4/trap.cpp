#include "trap.hpp"
#include "field.hpp"
#include "entitymanager.hpp"
#include "hero.hpp"
#include <iostream>
#include <cmath>

TrapSpell::TrapSpell(int dmg) : base_damage(dmg) {}

void TrapSpell::upgrade() {
    base_damage += 50;
    is_upgraded = true;
    upgrade_count++;
}

bool TrapSpell::cast(Field& field, EntityManager& roster, Hero& player, int tx, int ty) {
    int hx = field.x, hy = field.y;
    if (std::abs(tx - hx) > 3 || std::abs(ty - hy) > 3) {
        std::cout << "Too far to place trap!\n";
        return false;
    }
    if (!field.canMoveTo(tx, ty)) {
        std::cout << "Cannot place trap here!\n";
        return false;
    }
    if (field.isValidPosition(tx, ty)) {
        field.box[ty][tx] = FieldCell(TRAP, ZERO, 0);
    }
    return true;
}

std::string TrapSpell::name() const {
    return is_upgraded ? "Trap+" : "Trap";
}

int TrapSpell::cost() const {
    return 15;
}

void TrapSpell::load_state(std::istream& in) {
    int saved_upgrades;
    in.read(reinterpret_cast<char*>(&saved_upgrades), sizeof(saved_upgrades));
    upgrade_count = saved_upgrades;
    is_upgraded = (saved_upgrades > 0);
    base_damage = 100 + (saved_upgrades * 50);
}