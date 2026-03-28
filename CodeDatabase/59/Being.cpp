#include "Being.h"

Being::Being(float health, int maxHealth, int damage, Coordinates position)
    : health(health), maxHealth(maxHealth), damage(damage), position(position) {}

float Being::attack() { return damage;
}

Coordinates Being::getPosition() const { return position; }
void Being::setPosition(Coordinates pos) { position = pos; }

float Being::getHealth() const { return health;
}
int Being::getMaxHealth() const { return maxHealth; }

void Being::takeDamage(float dmg) { health -= dmg; }
void Being::move(Coordinates pos) { setPosition(pos);
}

json Being::serializeBase() const {
    json j;
    j["health"] = health;
    j["maxHealth"] = maxHealth;
    j["damage"] = damage;
    j["position"] = position.toJson();
    return j;
}

void Being::deserializeBase(const json& j) {
    j.at("health").get_to(health);
    j.at("maxHealth").get_to(maxHealth);
    j.at("damage").get_to(damage);
    position.fromJson(j.at("position"));
}