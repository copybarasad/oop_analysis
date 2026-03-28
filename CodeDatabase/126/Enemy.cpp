#include "../../headers/objects/Enemy.h"
#include <sstream>

using namespace std;

Enemy::Enemy(int x, int y, int lvl) : position(x, y), level(lvl) {
    health = 50 + (lvl - 1) * 10;
    damage = 10 + (lvl - 1) * 2;
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::takeDamage(int mount_damage) {
    health -= mount_damage;
    if (health < 0) health = 0;
}

void Enemy::moving(const Position& new_pos) {
    position = new_pos;
}

int Enemy::getHealth() const { return health; }

int Enemy::getDamage() const { return damage; }

Position Enemy::getPosition() const { return position; }

int Enemy::getLevel() const { return level; }

string Enemy::serialize() const {
    return "Enemy " + position.toString() + " " + to_string(health) + " " + 
           to_string(damage) + " " + to_string(level);
}

shared_ptr<Enemy> Enemy::deserialize(const string& data) {
    stringstream ss(data);
    string type;
    ss >> type;
    if (type != "Enemy") throw invalid_argument("Not an Enemy");

    string posStr;
    int health, damage, level;
    if (!(ss >> posStr >> health >> damage >> level)) {
        throw invalid_argument("Invalid enemy data format");
    }

    Position pos = Position::fromString(posStr);
    auto enemy = make_shared<Enemy>(pos.getX(), pos.getY(), level);
    enemy->health = health;
    enemy->damage = damage;
    return enemy;
}