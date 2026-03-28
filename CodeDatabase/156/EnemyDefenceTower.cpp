//
// Created by Artem on 30.10.2025.
//

#include "EnemyDefenceTower.h"

const DirectDamageSpell &EnemyDefenceTower::getDamageSpell() const noexcept {
    return this->spell;
}

int EnemyDefenceTower::getAttackRadius() const noexcept {
    return this->attackRadius;
}

int EnemyDefenceTower::getAttackInterval() const noexcept {
    return this->attackInterval;
}

std::unique_ptr<Entity> EnemyDefenceTower::clone() const {
    return std::make_unique<EnemyDefenceTower>(*this);
}

std::string EnemyDefenceTower::serialize() {
    std::string result = "{";
    result += "xCoordinate:" + std::to_string(this->xCoordinate) + ",";
    result += "yCoordinate:" + std::to_string(this->yCoordinate) + ",";
    result += "healthPoint:" + std::to_string(this->healthPoints) + ",";
    result += "attackRadius:" + std::to_string(this->attackRadius) + ",";
    result += "attackInterval:" + std::to_string(this->attackInterval)+"}";
    return result;
}

EnemyDefenceTower* EnemyDefenceTower::deserialize(std::map<std::string, std::string> fields) noexcept {
    int x = std::stoi(fields["xCoordinate"]);
    int y = std::stoi(fields["yCoordinate"]);
    int health = std::stoi(fields["healthPoint"]);
    int radius = std::stoi(fields["attackRadius"]);
    int interval = std::stoi(fields["attackInterval"]);
    return new EnemyDefenceTower(x, y, health, radius, interval);
}
