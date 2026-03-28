#include "effect.hpp"

const int DEFAULT_LIFETIME = 1;

Effect::Effect(int initial_damage, std::string name):
    initial_damage(initial_damage),
    tick_damage(0),
    slowing_down(false),
    name(name),
    lifetime(DEFAULT_LIFETIME) 
{}

Effect::Effect(int initial_damage, int tick_damage, bool slowing_down, std::string name):
    initial_damage(initial_damage),
    tick_damage(tick_damage),
    slowing_down(slowing_down),
    name(name),
    lifetime(DEFAULT_LIFETIME)
{}

Effect::Effect(int initial_damage, int tick_damage, bool slowing_down, std::string name, int lifetime):
    initial_damage(initial_damage),
    tick_damage(tick_damage),
    slowing_down(slowing_down),
    name(name),
    lifetime(lifetime) 
{}

Effect::Effect(const EffectImage& img):
    initial_damage_done(img.initial_damage_done),
    initial_damage(img.initial_damage),
    tick_damage(img.tick_damage),
    slowing_down(img.slowing_down),
    name(img.name),
    lifetime(img.lifetime)
{}

EffectImage Effect::get_data_image() const {
    return EffectImage(
        name,
        initial_damage_done,
        lifetime,
        initial_damage,
        tick_damage,
        slowing_down
    );
}

bool Effect::operator<(const Effect& other) const {
    return this->name < other.name;
}

int Effect::tick() {
    return lifetime--;
}

bool Effect::expired() {
    return lifetime <= 0;
}

bool Effect::is_slowing_down() {
    return slowing_down;
}

int Effect::apply_to_health(int cur_health_points) {
    if (!initial_damage_done) {
        initial_damage_done = true;
        return cur_health_points + initial_damage;
    } else {
        return cur_health_points + tick_damage;
    }
}