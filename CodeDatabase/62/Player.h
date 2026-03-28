#pragma once
#include "Entity.h"

class Player: public Entity {
private:
    int stuck_moves_ = 0;
public:
    Player(std::string texture_name, float speed, int health, int damage) : Entity(texture_name, speed, health, damage) {}
    ~Player() = default;

    void attack() {
        state_ = State::Attacking;
    }

    void stuck() {
        stuck_moves_++;
    }

    int getStuckedMoves() const {
        return stuck_moves_;
    }

    void unstuck() {
        stuck_moves_--;
    }
};