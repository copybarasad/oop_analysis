#include "enemy.hpp"

Enemy::Enemy(int hp, int maxHP) : Creature(hp, maxHP) {
    Creature::setTeam(Team::Enemy);
}
Enemy::Enemy(const Enemy &enemy) : Creature(enemy) {}
Enemy::Enemy(bytedump bytes) : Creature(bytes) {}

std::string Enemy::chr() { return "*"; }
Color Enemy::baseColor() { return Color::red(); }
