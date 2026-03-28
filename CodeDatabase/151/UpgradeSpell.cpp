#include "UpgradeSpell.h"
#include "GameField.h"

UpgradeSpell::UpgradeSpell(const std::string& name, int strength, int manaCost)
    : Spell(name, manaCost), upgradeStrength(strength) {
}

bool UpgradeSpell::cast(GameField* field, const Position& casterPos, const Position& targetPos) {
    // Заклинание улучшения добавляет улучшение игроку
    field->getPlayer()->addUpgrade(upgradeStrength);
    return true;
}

Spell* UpgradeSpell::clone() const {
    return new UpgradeSpell(*this);
}

int UpgradeSpell::getUpgradeStrength() const {
    return upgradeStrength;
}