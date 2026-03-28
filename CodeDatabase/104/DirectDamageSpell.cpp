#include "DirectDamageSpell.h"
#include "Player.h"
#include "GameField.h"
#include <iostream>

// Конструктор заклинания прямого урона
DirectDamageSpell::DirectDamageSpell(int damage, int range, int mana_cost)
    : damage_(damage), range_(range), mana_cost_(mana_cost), isAreaEnhanced_(false) {
}

// Проверка возможности применения заклинания
bool DirectDamageSpell::canCast(const Player& caster, const GameField& field,
    const SpellCastInfo& cast_info) const {
    if (!caster.canAffordSpell(mana_cost_)) {
        return false;
    }

    Position caster_pos = caster.getPosition();
    if (!caster_pos.isWithinRange(cast_info.target_position, range_)) {
        return false;
    }
    return field.hasEnemyAt(cast_info.target_position.getX(), cast_info.target_position.getY()) ||
        field.hasTowerAt(cast_info.target_position.getX(), cast_info.target_position.getY());
}

// Применение заклинания прямого урона
bool DirectDamageSpell::cast(Player& caster, GameField& field,
    const SpellCastInfo& cast_info) {
    if (!canCast(caster, field, cast_info)) {
        return false;
    }
    caster.spendMana(mana_cost_);

    bool hit_anything = false;

    if (isAreaEnhanced_) {
        // Улучшенная версия - бьет по области
        int center_x = cast_info.target_position.getX();
        int center_y = cast_info.target_position.getY();

        std::cout << "Enhanced Direct Damage spell unleashed!\n";

        for (int x = center_x - 1; x <= center_x + 1; ++x) {
            for (int y = center_y - 1; y <= center_y + 1; ++y) {
                if (field.hasEnemyAt(x, y) || field.hasTowerAt(x, y)) {
                    field.damageAtPosition(Position(x, y), damage_);
                    hit_anything = true;
                }
            }
        }
    }
    else {
        // Обычная версия - бьет одну цель
        if (field.hasEnemyAt(cast_info.target_position.getX(), cast_info.target_position.getY()) ||
            field.hasTowerAt(cast_info.target_position.getX(), cast_info.target_position.getY())) {
            field.damageAtPosition(cast_info.target_position, damage_);
            hit_anything = true;
        }
    }

    if (!hit_anything) {
        std::cout << "Spell hit empty space.\n";
    }

    return true;
}

// Создание копии заклинания
std::unique_ptr<SpellCard> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(*this);
}