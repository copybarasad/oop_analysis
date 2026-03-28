#include "ally.h"
#include "enemy.h"
#include "game_field.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include <cmath>
#include <cstdlib>
#include <limits>

Ally::Ally(int health, int damage, int x, int y) 
    : GameEntity(health, damage), position(x, y) {}

std::pair<int, int> Ally::getPosition() const {
    return position;
}

void Ally::setPosition(int x, int y) {
    position = {x, y};
}

std::pair<int, int> Ally::calculateMoveTowardsClosestEnemy(
    const std::vector<Enemy>& enemies,
    const GameField& field,
    const std::vector<EnemyBuilding>& buildings,
    const std::vector<EnemyTower>& towers) const {

    if (enemies.empty()) {
        return position;
    }

    int closestDist = std::numeric_limits<int>::max();
    std::pair<int, int> closestPos = position;

    for (const auto& enemy : enemies) {
        auto enemyPos = enemy.getPosition();
        int dist = std::abs(enemyPos.first - position.first) +
                   std::abs(enemyPos.second - position.second);

        if (dist < closestDist) {
            closestDist = dist;
            closestPos = enemyPos;
        }
    }

    int dx = 0, dy = 0;

    if (closestPos.first < position.first) dx = -1;
    else if (closestPos.first > position.first) dx = 1;

    if (closestPos.second < position.second) dy = -1;
    else if (closestPos.second > position.second) dy = 1;

    if (dx != 0 && dy != 0) {
        if (rand() % 2 == 0) dx = 0;
        else dy = 0;
    }

    int newX = position.first + dx;
    int newY = position.second + dy;

    if (field.isValidPosition(newX, newY) &&
        field.isPassable(newX, newY) &&
        !field.isSlowing(newX, newY) &&
        !field.isStructureAt(newX, newY, buildings, towers)) {
        return {newX, newY};
    }

    return position;
}