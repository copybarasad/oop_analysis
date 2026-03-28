//
// Created by Artem on 24.09.2025.
//

#include "EnemySpawnerBuilding.h"


int EnemySpawnerBuilding::getSpawnInterval() noexcept {
    return this->spawnInterval;
}

std::unique_ptr<Entity> EnemySpawnerBuilding::clone() const {
    return std::make_unique<EnemySpawnerBuilding>(*this);
}

std::string EnemySpawnerBuilding::serialize() {
    std::string result = "{";
    result += "xCoordinate:" + std::to_string(this->xCoordinate) + ",";
    result += "yCoordinate:" + std::to_string(this->yCoordinate) + ",";
    result += "healthPoint:" + std::to_string(this->healthPoints) + ",";
    result += "spawnInterval:" + std::to_string(this->spawnInterval);
    result += "}";
    return result;
}

EnemySpawnerBuilding* EnemySpawnerBuilding::deserialize(std::map<std::string, std::string> fields) noexcept {
    int x = std::stoi(fields["xCoordinate"]);
    int y = std::stoi(fields["yCoordinate"]);
    int health = std::stoi(fields["healthPoint"]);
    int interval = std::stoi(fields["spawnInterval"]);

    return new EnemySpawnerBuilding(x, y, health, interval);
}
