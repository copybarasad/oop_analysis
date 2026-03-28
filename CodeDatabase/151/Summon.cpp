#include "Summon.h"
#include "GameField.h"
#include <cmath>
#include <iostream>

SummonSpell::SummonSpell(const std::string& name, int alliesCount, int range, int manaCost)
    : Spell(name, manaCost), baseAlliesCount(alliesCount),
    currentAlliesCount(alliesCount), range(range) {
}

bool SummonSpell::cast(GameField* field, const Position& casterPos, const Position& targetPos) {
    // Проверяем расстояние
    int dx = std::abs(casterPos.x - targetPos.x);
    int dy = std::abs(casterPos.y - targetPos.y);
    if (dx > range || dy > range) {
        std::cout << "Target is too far! Range: " << range << "\n";
        return false;
    }

    // Упрощённая реализация: наносим урон по области вокруг цели
    // Вместо призыва союзников создаём магический взрыв
    int damage = 20 + (currentAlliesCount * 5); // Урон зависит от "количества союзников"
    int areaSize = 1 + currentAlliesCount; // Размер области зависит от улучшений

    std::cout << "Summoning magical forces! ";
    std::cout << "Damage: " << damage << ", Area: " << areaSize << "x" << areaSize << "\n";

    field->applyDamageInArea(targetPos, areaSize, damage);
    return true;
}

Spell* SummonSpell::clone() const {
    return new SummonSpell(*this);
}

void SummonSpell::upgradeAlliesCount(int additionalCount) {
    currentAlliesCount += additionalCount;
    std::cout << "Summon power increased! Now summons " << currentAlliesCount << " magical forces!\n";
}

int SummonSpell::getCurrentAlliesCount() const {
    return currentAlliesCount;
}