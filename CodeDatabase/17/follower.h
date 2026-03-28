#ifndef FOLLOWER_H
#define FOLLOWER_H

#include "entity.h"
#include "entity_type.h"
class Follower : public Entity {
public:
    Follower(int start_x, int start_y, int health = 50, int damage = 10)
        : Entity(EntityType::FOLLOWER, start_x, start_y, health, damage) {}

    std::string getStatusString() const override {
        return "HP: " + std::to_string(getHealth()) + " DMG: " + std::to_string(getDamage());
    }
};

#endif