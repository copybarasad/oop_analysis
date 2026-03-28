#include "fireball_spell.hpp"
#include "field_handler.hpp"

const int DEFAULT_R = 2;

FireballSpell::FireballSpell(): DamageSpell(RANGE, DEFAULT_R, { Bullet() }, 'F') {};

int FireballSpell::apply_enhancement(int current_lvl) {
    area_r = DEFAULT_R * current_lvl;

    return 0;
}

bool FireballSpell::use(FieldHandler& field_handler, const Position& from, const Position& target) {
    if (Position::euclidean_sq_distance(from, target) > range * range) return false;

    bool spell_used = false;

    for (int i = target.x - area_r; i < target.x + area_r; i++) {
        for (int j = target.y - area_r; j < target.y + area_r; j++) {
            if (!field_handler.get_field().in_bounds({ i, j })) continue;

            spell_used = true;

            if (IEntity* entity = dynamic_cast<IEntity*>(field_handler.get_obj({ i, j }))) {
                if (Position::manhattan_distance(target, { i, j }) <= area_r) {
                    for (Effect eff: effects) {
                        entity->add_effect(eff, true);
                    }
                }
            }
        }
    }

    return spell_used;
}