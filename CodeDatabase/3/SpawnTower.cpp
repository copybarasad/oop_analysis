#include "GameObjects/Entities/SpawnTower.hpp"
#include <iostream>


SpawnTower::SpawnTower() {}

Enemy SpawnTower::spawnEnemy(Coordinates pos) {
    Enemy e;
    e.setMaxHP(enemyStats.maxHP);
    e.setHealthPoints(enemyStats.hp);
    e.setDamagePoints(enemyStats.dmg);
    e.setPosition(pos);
    e.setDisabled(false);
    return e;
}

Coordinates SpawnTower::getPosition() {
    return position;
};

void SpawnTower::setPositoin(Coordinates pos) {
    position = pos;
}

void SpawnTower::setEnemyStats(EnemyDTO es) {
    enemyStats = es;
}

EnemyDTO SpawnTower::getEnemyStats() {
    return enemyStats;
}

