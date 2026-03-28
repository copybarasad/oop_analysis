#include "EnemyTower.h"
#include "Field.h"
#include "Player.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>

EnemyTower::EnemyTower(int radius, int damage, int cooldown, int hp)
    : attackRadius_(radius), damage_(damage), cooldown_(cooldown), 
      currentCooldown_(0), hp_(hp), maxHp_(hp) {}

void EnemyTower::tick() {
    if (currentCooldown_ > 0) --currentCooldown_;
}

bool EnemyTower::canAttack() const {
    return currentCooldown_ == 0 && isAlive();
}

void EnemyTower::resetCooldown() {
    currentCooldown_ = cooldown_;
}

int EnemyTower::getDamage() const {
    return damage_;
}

bool EnemyTower::attackPlayerIfInRange(Field& field, Player& player) {
    if (!canAttack()) return false;

    auto towers = field.findTowers();
    auto playerPos = field.findPlayer();
    
    for (const auto& [tx, ty] : towers) {
        auto tower = field.getCell(tx, ty).getTower();
        if (tower.get() == this) {
            double dist = std::hypot(playerPos.first - tx, playerPos.second - ty);
            if (dist <= attackRadius_) {
                player.takeDamage(damage_);
                resetCooldown();
                std::cout << "Башня атаковала игрока! Урон: " << damage_ << std::endl;
                return true;
            }
            break;
        }
    }
    
    return false;
}

int EnemyTower::getHp() const { return hp_; }
int EnemyTower::getMaxHp() const { return maxHp_; }
bool EnemyTower::isAlive() const { return hp_ > 0; }
void EnemyTower::takeDamage(int damage) { 
    hp_ = std::max(0, hp_ - damage); 
}

std::tuple<int, int, int, int, int> EnemyTower::parseData(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 5) {
        return {
            std::stoi(tokens[0]), 
            std::stoi(tokens[1]), 
            std::stoi(tokens[2]), 
            std::stoi(tokens[3]), 
            std::stoi(tokens[4]) 
        };
    }
    
    return {40, 40, 10, 0, 4};
}