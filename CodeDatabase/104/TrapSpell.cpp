#include "TrapSpell.h"
#include "Player.h"
#include "GameField.h"
#include <iostream>

// Конструктор заклинания ловушки - ТОЛЬКО 2 АРГУМЕНТА!
TrapSpell::TrapSpell(int damage, int mana_cost)
    : damage_(damage), mana_cost_(mana_cost), isAreaEnhanced_(false) {
}

// Проверка возможности применения заклинания
bool TrapSpell::canCast(const Player& caster, const GameField& field,
    const SpellCastInfo& cast_info) const {
    if (!caster.canAffordSpell(mana_cost_)) {
        return false;
    }

    // Ловушка не проверяет расстояние - можно ставить в любую видимую клетку
    // Проверяем только возможность размещения

    if (isAreaEnhanced_) {
        // Для улучшенной версии проверяем возможность размещения в области
        for (int x = cast_info.target_position.getX() - 1; x <= cast_info.target_position.getX() + 1; ++x) {
            for (int y = cast_info.target_position.getY() - 1; y <= cast_info.target_position.getY() + 1; ++y) {
                if (field.canPlaceTrapAt(x, y)) {
                    return true;
                }
            }
        }
        return false;
    }
    else {
        // Для обычной версии проверяем одну клетку
        return field.canPlaceTrapAt(cast_info.target_position.getX(),
            cast_info.target_position.getY());
    }
}

// Применение заклинания ловушки
bool TrapSpell::cast(Player& caster, GameField& field,
    const SpellCastInfo& cast_info) {
    if (!canCast(caster, field, cast_info)) {
        return false;
    }

    caster.spendMana(mana_cost_);

    if (isAreaEnhanced_) {
        // Улучшенная версия - создает мины в области 3x3
        std::cout << "Placed area trap (mine)!\n";
        for (int x = cast_info.target_position.getX() - 1; x <= cast_info.target_position.getX() + 1; ++x) {
            for (int y = cast_info.target_position.getY() - 1; y <= cast_info.target_position.getY() + 1; ++y) {
                if (field.canPlaceTrapAt(x, y)) {
                    field.placeTrap(Position(x, y), damage_);
                }
            }
        }
    }
    else {
        // Обычная версия - одна ловушка
        field.placeTrap(cast_info.target_position, damage_);
        std::cout << "Trap placed!\n";
    }
    return true;
}

// Создание копии заклинания
std::unique_ptr<SpellCard> TrapSpell::clone() const {
    return std::make_unique<TrapSpell>(*this);
}