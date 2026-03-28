#include "entity.hpp"

Entity::Entity(int health_points) {
    this->health_points = health_points;
}

std::vector<EffectImage> Entity::get_effects_img() {
    std::vector<EffectImage> effects_img;

    for (const Effect& eff: applied_effects) {
        effects_img.push_back(eff.get_data_image());
    }

    return effects_img;
}

bool Entity::is_alive() const {
    return health_points > 0;
}

void Entity::add_effect(Effect effect, bool immediate_apply) {
    applied_effects.push_back(effect);

    if (immediate_apply) {
        apply_effect(*--applied_effects.end());
    }
}

void Entity::add_effects(std::set<Effect> effects, bool immediate_apply) {
    for (auto eff: effects) {
        add_effect(eff, immediate_apply);
    }
}

void Entity::apply_effects() {
    reset_temp_effects();
    std::vector<Effect> updated_effects;

    for (Effect &effect: applied_effects) {
        if (!effect.expired()) {
            apply_effect(effect);
            updated_effects.push_back(effect);
        }
    }

    applied_effects = updated_effects;
}