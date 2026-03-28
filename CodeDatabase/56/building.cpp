#include "building.h"

Building::Building(int X, int Y, int hp, int cooldown, int spawn) 
    : GameObject(X, Y, hp, "none", 0, 0), cooldown_spawn(cooldown), spawn_timer(spawn) {
}

int Building::getDamage() {
    return 0;
}

int Building::getCooldown() {
    return cooldown_spawn;
}

int Building::getSpawn() {
    return spawn_timer;
}
nlohmann::json Building::toJson() const {
    return {
        {"x", x},
        {"y", y},
        {"health", HP},
        {"cooldown_spawn", cooldown_spawn},
        {"spawn_timer", spawn_timer},
        {"combat_type", type_combat},
        {"range", ranged},
        {"frozen", frozen},
        {"alive", flag_life}
    };
}

void Building::fromJson(const nlohmann::json& j) {
    x = j["x"];
    y = j["y"];
    HP = j["health"];
    cooldown_spawn = j["cooldown_spawn"];
    spawn_timer = j["spawn_timer"];
    type_combat = j["combat_type"];
    ranged = j["range"];
    frozen = j["frozen"];
    flag_life = j["alive"];
}
void Building::setSpawnTimer(int a) {
    spawn_timer = a;
}