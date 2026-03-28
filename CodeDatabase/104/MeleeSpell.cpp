#include "MeleeSpell.h"
#include "Player.h"
#include "GameField.h"
#include <iostream>

// Конструктор - ТОЛЬКО damage и mana_cost!
MeleeSpell::MeleeSpell(int damage, int mana_cost)
    : damage_(damage), mana_cost_(mana_cost), isRangeEnhanced_(false) {
}

// MeleeSpell всегда можно кастовать если хватает маны - не требует цели
bool MeleeSpell::canCast(const Player& caster, const GameField& field,
    const SpellCastInfo& cast_info) const {
    return caster.canAffordSpell(mana_cost_);
}

// Применение заклинания ближнего боя - ИГНОРИРУЕМ target_position!
bool MeleeSpell::cast(Player& caster, GameField& field,
    const SpellCastInfo& cast_info) {
    if (!canCast(caster, field, cast_info)) {
        return false;
    }

    caster.spendMana(mana_cost_);

    Position player_pos = caster.getPosition();
    int center_x = player_pos.getX();
    int center_y = player_pos.getY();

    int area_size = isRangeEnhanced_ ? 2 : 1;
    bool hit_anything = false;

    std::cout << "Melee spell unleashed around you!\n";

    // Наносим урон по области вокруг игрока
    for (int x = center_x - area_size; x <= center_x + area_size; ++x) {
        for (int y = center_y - area_size; y <= center_y + area_size; ++y) {
            if (x == center_x && y == center_y) {
                continue; // Пропускаем клетку игрока
            }

            // Проверяем, есть ли что-то в клетке перед нанесением урона
            if (field.hasEnemyAt(x, y) || field.hasTowerAt(x, y)) {
                field.damageAtPosition(Position(x, y), damage_);
                hit_anything = true;
            }
        }
    }

    if (!hit_anything) {
        std::cout << "Melee spell hit empty space around you.\n";
    }

    return true;
}

// Создание копии заклинания
std::unique_ptr<SpellCard> MeleeSpell::clone() const {
    return std::make_unique<MeleeSpell>(*this);
}