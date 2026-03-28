#include "Enemy.hpp"


Enemy::Enemy() : Enemy(1) {}

Enemy::Enemy(int enemyLevel) {
    this->enemyLevel = enemyLevel;
    enemyHealth.setMaxHealth(GlobalGameConfig::enemyHealth * enemyLevel * GlobalGameConfig::difficulty);
    enemyHealth.restoreHealth();
    enemyAttack.setAttack(GlobalGameConfig::enemyAttack * enemyLevel * GlobalGameConfig::difficulty);
}


int Enemy::getDamage() {
    return enemyAttack.getAttack();
}


int Enemy::getLevel() {
    return this->enemyLevel;
}


std::pair<int, int> Enemy::getHealth() {
    return std::make_pair(enemyHealth.getCurrentHealth(), enemyHealth.getMaxHealth());
}


void Enemy::causeDamage(int damage) {
    enemyHealth.reduseCurrentHealth(damage);
}

bool Enemy::alive() const {
    return enemyHealth.getCurrentHealth();
}

char Enemy::returnEntitySymbol() {
    return 'E';
}

EnemySaveData Enemy::getEnemySaveData() {
    EnemySaveData data;
    data.enemyHealth = this->enemyHealth.getHealthSavedata();
    data.enemyAttack = this->enemyAttack.getAttackSavedata();
    data.enemyLevel = this->enemyLevel;
    return data;
}


void Enemy::setEnemySaveData(EnemySaveData data) {
    this->enemyAttack.setAttackSaveData(data.enemyAttack);
    this->enemyHealth.setHealthSavedata(data.enemyHealth);
    this->enemyLevel = data.enemyLevel;
}