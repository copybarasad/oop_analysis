#include "zap_spell.hpp"
#include "field_handler.hpp"

const int DEFAULT_ZAP_DAMAGE = -10;

ZapSpell::ZapSpell(): 
    DamageSpell(RANGE, 0, { Effect(DEFAULT_ZAP_DAMAGE, "zap_spell") }, 'Z') 
{}

ZapSpell::ZapSpell(int damage): 
    DamageSpell(RANGE, 0, { Effect(damage, "zap_spell") }, 'Z') 
{}

int ZapSpell::apply_enhancement(int current_lvl) {
    effects = { Effect(DEFAULT_ZAP_DAMAGE + DEFAULT_ZAP_DAMAGE * current_lvl, "zap_spell") };

    return 0;
}

bool ZapSpell::use(FieldHandler& field_handler, const Position& from, const Position& target) {
    if (Position::euclidean_sq_distance(from, target) > range * range) return false;

    if (IEntity* entity = dynamic_cast<IEntity*>(field_handler.get_obj(target))) {
        for (Effect eff: effects) {
            entity->add_effect(eff, true);
        }
        return true;
    }

    return false;
}