#include "building.hpp"

Building::Building(
    std::shared_ptr<ISpell> spell, 
    int health_points, 
    int cooldown, 
    int timer
): 
    Entity(health_points), HasHand(1, spell), cooldown(cooldown), timer(timer)
{}

void Building::reset_temp_effects() {}

void Building::reset_timer() {
    timer = 0;
}

bool Building::is_move_available() const {
    return timer >= cooldown;
}

void Building::apply_effect(Effect &effect) {
    health_points = effect.apply_to_health(health_points);

    printf("[%c] got [%s],  health=%d\n", screen_symbol, effect.name.c_str(), health_points);

    effect.tick();
}