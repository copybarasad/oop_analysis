#pragma once
#include "Entity.h"

class Enemy : public Entity {
    public:
    Enemy(std::pair<int, int> position, int maxHealth, int damage) : Entity(position, maxHealth, maxHealth, damage) {}
    Enemy() : Entity({0,0},0,0,0) {}
    json to_json() override {
        return Entity::to_json();
    }
    bool from_json(json& j) override {
        return Entity::from_json(j);
    }
};
