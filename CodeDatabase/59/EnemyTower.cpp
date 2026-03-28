#include "EnemyTower.h"

bool EnemyTower::can_attack(Coordinates target_pos) {
    if (curr_delay != 0) return false;
    return spell.doesSpellReach(position, target_pos);
}

float EnemyTower::attack() {
    curr_delay = delay;
    return spell.getDamage();
}

json EnemyTower::toJson() const {
    return json{
            {"health", health},
            {"maxHealth", maxHealth},
            {"position", position.toJson()},
            {"delay", delay},
            {"curr_delay", curr_delay},
            {"spell", spell.toJson()}
    };
}

void EnemyTower::fromJson(const json& j) {
    j.at("health").get_to(health);
    j.at("maxHealth").get_to(maxHealth);
    position.fromJson(j.at("position"));
    j.at("delay").get_to(delay);
    j.at("curr_delay").get_to(curr_delay);

    spell.fromJson(j.at("spell"));
}