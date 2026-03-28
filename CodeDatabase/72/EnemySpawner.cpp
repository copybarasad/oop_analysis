//
// Created by bob on 10/15/25.
//

#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(std::shared_ptr<Field> field): field(field) {}

void EnemySpawner::spawnEnemies() {
    for (auto &build : field->getEnemiesBuild()) {
        if (build.isSpawn(field->getSteps())) {
            Enemy enemy = build.spawnEnemy();
            Position pos = field->generatePositionEnemy(build.getPosition());
            enemy.setPosition(pos);
            field->addEnemy(enemy);
        }
    }
}

