#include "Enemy.h"
#include <iostream>

Enemy::Enemy(int health, int damage, int x, int y)
    : Entity(health, damage, x, y) {
}

void Enemy::log() const {
    std::cout << "[Enemy] { x: " << pos.x
        << ", y: " << pos.y
        << " }, HP: " << health
        << ", Dmg: " << getDamage()
        << std::endl;
}

json Enemy::serialize() const {
    json j;
    j["x"] = pos.x;
    j["y"] = pos.y;
    j["health"] = health;
    j["damage"] = damage;

    return j;
}

void Enemy::deserialize(const json& j) {
    pos.x = j.at("x").get<int>();
    pos.y = j.at("y").get<int>();
    health = j.at("health").get<int>();
    damage = j.at("damage").get<int>();
}