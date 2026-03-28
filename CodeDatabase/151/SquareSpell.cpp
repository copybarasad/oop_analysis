#include "SquareSpell.h"
#include "GameField.h"
#include <cmath>

SquareSpell::SquareSpell(const std::string& name, int damage, int areaSize, int range, int manaCost)
    : Spell(name, manaCost), damage(damage), baseAreaSize(areaSize),
    currentAreaSize(areaSize), range(range) {
}

bool SquareSpell::cast(GameField* field, const Position& casterPos, const Position& targetPos) {
    // Проверяем расстояние
    int dx = std::abs(casterPos.x - targetPos.x);
    int dy = std::abs(casterPos.y - targetPos.y);
    if (dx > range || dy > range) {
        return false;
    }

    // Наносим урон по площади
    field->applyDamageInArea(targetPos, currentAreaSize, damage);
    return true;
}

Spell* SquareSpell::clone() const {
    return new SquareSpell(*this);
}

void SquareSpell::upgradeAreaSize(int additionalSize) {
    currentAreaSize += additionalSize;
}

int SquareSpell::getCurrentAreaSize() const {
    return currentAreaSize;
}