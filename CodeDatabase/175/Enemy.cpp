#include "Enemy.h"
#include <algorithm>
#include <sstream>
#include <vector>

Enemy::Enemy(int hp, int damage) : hp_(hp), maxHp_(hp), damage_(damage) {}

int Enemy::getHp() const { return hp_; }
int Enemy::getMaxHp() const { return maxHp_; }
int Enemy::getDamage() const { return damage_; }
bool Enemy::isAlive() const { return hp_ > 0; }
void Enemy::takeDamage(int dmg) { hp_ = std::max(0, hp_ - dmg); }

std::pair<int, int> Enemy::parseData(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 2) {
        return {std::stoi(tokens[0]), std::stoi(tokens[1])};
    }
    
    return {30, 5};
}