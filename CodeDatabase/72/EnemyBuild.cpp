//
// Created by bob on 10/5/25.
//

#include "EnemyBuild.h"

#include <stdexcept>

#define ERROR_INVALID_COUNT "The number of moves must be greater than zero."
#define DEFAULT_DAMAGE_ENEMY_BUILD 0
#define DEFAULT_DAMAGE_ENEMY_POSITION {0, 0}
#define DEFAULT_ENEMY_POSITION {0, 0}

EnemyBuild::EnemyBuild(const int & enemiesHealth, const int & enemiesDamage,const int & health ,const int & spawnCount)
    :Character(DEFAULT_DAMAGE_ENEMY_BUILD,DEFAULT_DAMAGE_ENEMY_POSITION , health), enemiesDamage(validateDamage(enemiesDamage)), enemiesHealth(validateHealth(enemiesHealth)),
    spawnCount(validatePositive(spawnCount, ERROR_INVALID_COUNT))
{}

bool EnemyBuild::isSpawn(const int &counterSteps) const {
    return validatePositive(counterSteps, ERROR_INVALID_COUNT) % this->spawnCount == 0;
}

Enemy EnemyBuild::spawnEnemy() const {
    return Enemy(enemiesDamage, DEFAULT_ENEMY_POSITION, enemiesHealth);
}

void EnemyBuild::setEnemiesDamage(const int &enemiesDamage) {
    this->enemiesDamage = this->validateDamage(enemiesDamage);
}

void EnemyBuild::setEnemiesHealth(const int &enemiesHealth) {
    this->enemiesHealth = this->validateHealth(enemiesHealth);
}

void EnemyBuild::setSpawnCount(const int &spawnCount) {
    this->spawnCount = validatePositive(spawnCount, ERROR_INVALID_COUNT);
}

int EnemyBuild::validatePositive(const int &value, const std::string &error) const {
    if (value > 0) return value;
    else throw std::runtime_error(error);
}

void EnemyBuild::load(Reader &reader) {
    reader.read(damage);
    reader.read(health);
    position.load(reader);
    reader.read(spawnCount);
    reader.read(enemiesDamage);
    reader.read(enemiesHealth);
}

void EnemyBuild::save(Saver &saver) const {
    saver.save(damage);
    saver.save(health);
    position.save(saver);
    saver.save(spawnCount);
    saver.save(enemiesDamage);
    saver.save(enemiesHealth);
}






