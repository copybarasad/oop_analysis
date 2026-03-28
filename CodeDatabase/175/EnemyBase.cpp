#include "EnemyBase.h"
#include <algorithm>
#include <sstream>
#include <vector>

EnemyBase::EnemyBase(int interval, int hp)
    : spawnInterval_(interval), turnsUntilSpawn_(interval), hp_(hp), maxHp_(hp) {}

void EnemyBase::tick() {
    if (turnsUntilSpawn_ > 0) --turnsUntilSpawn_;
}

bool EnemyBase::canSpawn() const {
    return turnsUntilSpawn_ == 0 && isAlive();
}

void EnemyBase::resetTimer() {
    turnsUntilSpawn_ = spawnInterval_;
}

int EnemyBase::getSpawnInterval() const {
    return spawnInterval_;
}

int EnemyBase::getHp() const { return hp_; }
int EnemyBase::getMaxHp() const { return maxHp_; }
bool EnemyBase::isAlive() const { return hp_ > 0; }
void EnemyBase::takeDamage(int damage) { 
    hp_ = std::max(0, hp_ - damage); 
}

std::tuple<int, int, int, int> EnemyBase::parseData(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 4) {
        return {
            std::stoi(tokens[0]),
            std::stoi(tokens[1]),
            std::stoi(tokens[2]), 
            std::stoi(tokens[3])  
        };
    }
    
    return {50, 50, 3, 3};
}