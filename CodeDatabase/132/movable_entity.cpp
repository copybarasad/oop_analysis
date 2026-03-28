#include "movable_entity.hpp"

MovableEntity::MovableEntity(int health_points): Entity(health_points) {}
MovableEntity::MovableEntity(int health_points, bool can_move): 
    Entity(health_points), can_move(can_move)
{}

void MovableEntity::apply_effect(Effect &effect) {
    health_points = effect.apply_to_health(health_points);
    can_move = !effect.is_slowing_down();

    printf("[%c] got [%s],  health=%d\n", screen_symbol, effect.name.c_str(), health_points);

    effect.tick();
}

void MovableEntity::reset_temp_effects() {
    can_move = true;
}

bool MovableEntity::is_move_available() const {
    return can_move;
}