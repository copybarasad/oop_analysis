#pragma once

#include "Coordinates.h"
#include "json.hpp"
using json = nlohmann::json;

class Being {
protected:
    float health;
    int maxHealth;
    int damage;
    Coordinates position;

    json serializeBase() const;
    void deserializeBase(const json& j);

public:
    Being(float health, int maxHealth, int damage, Coordinates position);
    virtual ~Being() = default;
    virtual float attack();

    Coordinates getPosition() const;
    void setPosition(Coordinates pos);

    float getHealth() const;
    int getMaxHealth() const;
    void takeDamage(float dmg);
    void move(Coordinates pos);

    int getBaseDamage() const { return damage;
}
    void setHealth(float h) { health = h;
}
    void setMaxHealth(int h) { maxHealth = h;
}
    void setDamage(int d) { damage = d; }
};