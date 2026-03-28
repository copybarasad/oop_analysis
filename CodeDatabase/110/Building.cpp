#include "Building.hpp"

Building::Building (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, size_t spawnCoolDownValue, size_t currentCoolDownValue=0,
    unsigned int damageValue, std::pair<unsigned int, unsigned int> enemyMinStats) :
    Entity(id, maxHealthValue, currentHealthValue, damageValue), spawnCoolDown(spawnCoolDownValue), currentCoolDown(currentCoolDownValue), enemyMinStats(enemyMinStats) {}

size_t Building::getMaxCoolDown () const { return spawnCoolDown; }

size_t Building::getSpawnCoolDown () const { return currentCoolDown; }

std::pair<unsigned int, unsigned int> Building::getEnemyMinStats() const { return enemyMinStats; }

Enemy* Building::spawnEnemy (size_t id) {
    unsigned int enemyHealth = generateEnemyHealth(enemyMinStats.first);
    unsigned int enemyDamage = generateEnemyDamage(enemyMinStats.second);

    currentCoolDown = spawnCoolDown;

    return new Enemy(id, enemyHealth, enemyHealth, enemyDamage);
}

unsigned int Building::calculateDamage () const { return 0; }

void Building::subCoolDown () { currentCoolDown = std::max(0, (int)currentCoolDown - 1); }

unsigned int Building::generateEnemyHealth (unsigned int enemyMinHealth) const {
    std::random_device rd;
    std::mt19937 gen(rd());

    unsigned int health = gen() % 15 + enemyMinHealth;
    return health;
}

unsigned int Building::generateEnemyDamage (unsigned int enemyMinDamage) const {
    std::random_device rd;
    std::mt19937 gen(rd());

    unsigned int damage = gen() % 5 + enemyMinDamage;
    return damage;
}