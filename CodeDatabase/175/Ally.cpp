#include "Ally.h"
#include <algorithm>
#include <sstream>
#include <vector>

Ally::Ally(int hp, int damage) : hp_(hp), maxHp_(hp), damage_(damage) {}

int Ally::getHp() const { return hp_; }
int Ally::getMaxHp() const { return maxHp_; }
int Ally::getDamage() const { return damage_; }
bool Ally::isAlive() const { return hp_ > 0; }
void Ally::takeDamage(int dmg) { hp_ = std::max(0, hp_ - dmg); }

std::pair<int, int> Ally::parseData(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 2) {
        return {std::stoi(tokens[0]), std::stoi(tokens[1])};
    }
    
    return {20, 8};
}