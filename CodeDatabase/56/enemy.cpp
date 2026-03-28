#include "enemy.h"

Enemy::Enemy(int X, int Y, int hp, int dam, int strong) 
    : GameObject(X, Y, hp, "melee", 1, 0), damage(dam), Strong(strong) {
}

int Enemy::getDamage() {
    return damage;
}
nlohmann::json Enemy::toJson() const {
    return {
        {"x", x},
        {"y", y},
        {"health", HP},
        {"damage", damage},
        {"Strong", Strong},
        {"combat_type", type_combat},
        {"range", ranged},
        {"frozen", frozen},
        {"alive", flag_life}
    };
}

void Enemy::fromJson(const nlohmann::json& j) {
    x = j["x"];
    y = j["y"];
    HP = j["health"];
    damage = j["damage"];
    Strong = j["Strong"];
    type_combat = j["combat_type"];
    ranged = j["range"];
    frozen = j["frozen"];
    flag_life = j["alive"];
}