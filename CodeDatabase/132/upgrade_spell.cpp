#include "upgrade_spell.hpp"

UpgradeSpell::UpgradeSpell() {};
bool UpgradeSpell::use(FieldHandler& field_handler, const Position& from, const Position& target) {
    return true;
}

int UpgradeSpell::apply_enhancement(int current_lvl) {
    return current_lvl + 1;
}