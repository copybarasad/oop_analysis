#include "AllyUnit.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "GameField.h"
#include "Constants.h"
#include <cmath>

using namespace std;

AllyUnit::AllyUnit(int x, int y)
    : Unit(DEFAULT_PLAYER_HEALTH / 2, DEFAULT_PLAYER_MELEE_DAMAGE / 2, x, y) {}

void AllyUnit::moveAI(const vector<shared_ptr<Enemy>>& enemies,
                      const vector<shared_ptr<EnemyBuilding>>& buildings,
                      const GameField& field) {

    Position closestTargetPos(-1, -1);
    int minDist = -1;

    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            int dist = abs(enemy->getPosition().getX() - position.getX()) +
                       abs(enemy->getPosition().getY() - position.getY());
            if (minDist == -1 || dist < minDist) {
                minDist = dist;
                closestTargetPos = enemy->getPosition();
            }
        }
    }

    for (const auto& building : buildings) {
        if (building && building->isAlive()) {
            int dist = abs(building->getPosition().getX() - position.getX()) +
                       abs(building->getPosition().getY() - position.getY());
            if (minDist == -1 || dist < minDist) {
                minDist = dist;
                closestTargetPos = building->getPosition();
            }
        }
    }

    if (closestTargetPos.getX() == -1) {
        return; 
    }

    int dx = closestTargetPos.getX() - position.getX();
    int dy = closestTargetPos.getY() - position.getY();


    int moveX = 0;
    int moveY = 0;

    if (dx != 0 || dy != 0) {
        if (abs(dx) > abs(dy)) {
            moveX = (dx > 0) ? 1 : -1;
        } else {
            moveY = (dy > 0) ? 1 : -1;
        }
    }

    int newX = position.getX() + moveX;
    int newY = position.getY() + moveY;

    if (field.isValidPosition(newX, newY) && field.isEmptyAt(newX, newY)) {
        position.setX(newX);
        position.setY(newY);
    }

    if (canAttackTarget(closestTargetPos)) {
        for (const auto& enemy : enemies) {
            if (enemy && enemy->getPosition() == closestTargetPos) {
                enemy->takeDamage(damage);
                return;
            }
        }
        for (const auto& building : buildings) {
            if (building && building->getPosition() == closestTargetPos) {
                building->takeDamage(damage);
                return;
            }
        }
    }
}

bool AllyUnit::canAttackTarget(const Position& targetPos) const {
    int dist = abs(targetPos.getX() - position.getX()) +
               abs(targetPos.getY() - position.getY());
    return dist <= 1;
}

CellType AllyUnit::getCellType() const {
    return CellType::ALLY;
}