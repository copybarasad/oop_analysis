#include "Entity.h"
#include <iostream>
#include <cmath>

void Entity::move(float dt) {
    float dx = target_x_ - x_;
    float dy = target_y_ - y_;
    float dist = std::sqrt(dx * dx + dy * dy);

    if(dist <= error_rate) {
        if(state_ == State::Moving) {
            state_ = State::Idle;
            x_ = target_x_;
            y_ = target_y_;
        }
        return;
    }
    
    state_ = State::Moving;

    if(dx < 0) {
        direction_ = -1;
    } else if (dx > 0) {
        direction_ = 1;
    }

    float dir_x = dx / dist;
    float dir_y = dy / dist;

    x_ += dir_x * dt * speed_;
    y_ += dir_y * dt * speed_;
};

void Entity::update(float dt) {
    move(dt);
}