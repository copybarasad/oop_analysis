#include "StraightSpell.h"
#include "GameField.h"

StraightSpell::StraightSpell(const std::string& name, int damage, int range, int manaCost)
    : Spell(name, manaCost), damage(damage), baseRange(range), currentRange(range) {
}

bool StraightSpell::cast(GameField* field, const Position& casterPos, const Position& targetPos) {
    // Проверяем расстояние
    int dx = std::abs(casterPos.x - targetPos.x);
    int dy = std::abs(casterPos.y - targetPos.y);
    if (dx > currentRange || dy > currentRange) {
        return false;
    }

    // Проверяем, есть ли враг или башня на целевой позиции
    if (field->isEnemyAtPosition(targetPos) || field->isTowerAtPosition(targetPos)) {
        // Наносим урон цели
        field->applyDamageToPosition(targetPos, damage);
        return true;
    }

    return false;
}

Spell* StraightSpell::clone() const {
    return new StraightSpell(*this);
}

void StraightSpell::upgradeRange(int additionalRange) {
    currentRange += additionalRange;
}

int StraightSpell::getCurrentRange() const {
    return currentRange;
}