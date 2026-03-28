#include "AreaDamageSpell.h"
#include "Player.h"
#include "GameField.h"
#include <iostream>

//  онструктор заклинани€ областного урона
AreaDamageSpell::AreaDamageSpell(int damage, int range, int mana_cost)
    : damage_(damage), range_(range), mana_cost_(mana_cost), isRangeEnhanced_(false) {
}

// ѕроверка возможности применени€ заклинани€
bool AreaDamageSpell::canCast(const Player& caster, const GameField& field,
    const SpellCastInfo& cast_info) const {
    if (!caster.canAffordSpell(mana_cost_)) {
        return false;
    }

    Position caster_pos = caster.getPosition();
    return caster_pos.isWithinRange(cast_info.target_position, range_);
}

// ѕрименение заклинани€ областного урона
bool AreaDamageSpell::cast(Player& caster, GameField& field,
    const SpellCastInfo& cast_info) {
    if (!canCast(caster, field, cast_info)) {
        return false;
    }

    caster.spendMana(mana_cost_);

    int center_x = cast_info.target_position.getX();
    int center_y = cast_info.target_position.getY();

    int area_size = isRangeEnhanced_ ? 2 : 1;
    bool hit_anything = false;

    std::cout << "Area spell unleashed!\n";

    for (int x = center_x - area_size; x <= center_x + area_size; ++x) {
        for (int y = center_y - area_size; y <= center_y + area_size; ++y) {
            // ѕровер€ем, есть ли цель перед нанесением урона
            if (field.hasEnemyAt(x, y) || field.hasTowerAt(x, y)) {
                field.damageAtPosition(Position(x, y), damage_);
                hit_anything = true;
            }
        }
    }

    if (!hit_anything) {
        std::cout << "Area spell hit empty space.\n";
    }

    return true;
}

// —оздание копии заклинани€
std::unique_ptr<SpellCard> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(*this);
}