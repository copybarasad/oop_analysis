#include "EnemyBuilding.h"
#include "Exceptions.h"

namespace Game {

EnemyBuilding::EnemyBuilding(Utils::Point location, int spawnIntervalTurns, int initialHealth) :
    position(location),
    spawnInterval(spawnIntervalTurns),
    turnsUntilSpawn(spawnIntervalTurns),
    health(initialHealth)
{
    if (spawnInterval <= 0) {
        this->spawnInterval = 1;
    }
}

EnemyBuilding::EnemyBuilding(std::istream& is) {
    deserialize(is);
}

const Utils::Point& EnemyBuilding::getPosition() const {
    return position;
}

bool EnemyBuilding::shouldSpawnEnemyThisTurn() {
    turnsUntilSpawn--;
    return turnsUntilSpawn <= 0;
}

void EnemyBuilding::resetSpawnTimer() {
    turnsUntilSpawn = spawnInterval;
}

bool EnemyBuilding::isAlive() const {
    return health > 0;
}

void EnemyBuilding::takeDamage(int amount) {
    health -= amount;
    if (health < 0) {
        health = 0;
    }
}

int EnemyBuilding::getHealth() const {
    return health;
}

void EnemyBuilding::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&position), sizeof(position));
    os.write(reinterpret_cast<const char*>(&spawnInterval), sizeof(spawnInterval));
    os.write(reinterpret_cast<const char*>(&turnsUntilSpawn), sizeof(turnsUntilSpawn));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
}

void EnemyBuilding::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&position), sizeof(position));
    is.read(reinterpret_cast<char*>(&spawnInterval), sizeof(spawnInterval));
    is.read(reinterpret_cast<char*>(&turnsUntilSpawn), sizeof(turnsUntilSpawn));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    if (is.fail()) throw FileReadException("Failed to read EnemyBuilding data.");
}

}