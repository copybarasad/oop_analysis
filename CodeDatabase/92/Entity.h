#pragma once
#include <utility>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Entity {
protected:
    float health;
    int maxHealth;
    int damage;
    std::pair<int, int> position;

public:
    Entity(std::pair<int, int> position, float health, int maxHealth, int damage);
    virtual ~Entity() = default;
    virtual float attack();

    std::pair<int, int> getPosition() const;
    void setPosition(std::pair<int, int> pos);

    float getHealth() { return health; }
    void changeHealth(float delta) { health += delta; }
    int getMaxHealth() { return maxHealth; };

    virtual json to_json();
    virtual bool from_json(json& j);
};
