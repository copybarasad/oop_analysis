#include "Enemy.h"
#include "Player.h"
#include "EnemyBuilding.h"
#include "AllyUnit.h"
#include "GameField.h"
#include "Constants.h"
#include <random>
#include <cmath>

using namespace std;

Enemy::Enemy(int x, int y) : Unit(DEFAULT_ENEMY_HEALTH, DEFAULT_ENEMY_DAMAGE, x, y) {}

void Enemy::moveAI(const Position& playerPosition,
                   const std::vector<std::shared_ptr<AllyUnit>>& allAllies,
                   const std::vector<std::shared_ptr<Enemy>>& allEnemies,
                   const std::vector<std::shared_ptr<EnemyBuilding>>& allBuildings,
                   GameField& field) {

    Position closestTargetPos = playerPosition;
    int minDist = std::abs(closestTargetPos.getX() - position.getX()) +
                  std::abs(closestTargetPos.getY() - position.getY());

    for (auto& ally : allAllies) {
        if (ally && ally->isAlive()) {
            int dist = std::abs(ally->getPosition().getX() - position.getX()) +
                       std::abs(ally->getPosition().getY() - position.getY());
            if (dist < minDist) {
                minDist = dist;
                closestTargetPos = ally->getPosition();
            }
        }
    }

    Position bestMove = findBestMove(closestTargetPos, allEnemies, allBuildings, allAllies, field);
    if (bestMove.getX() != -1) {
        position = bestMove;
    }

    if (field.getCellType(position.getX(), position.getY()) == CellType::TRAP) {
        field.triggerTrapAt(position.getX(), position.getY(), *this);
    }
}

bool Enemy::canAttackTarget(const Position& targetPos) const {
    int dist = std::abs(targetPos.getX() - position.getX()) +
               std::abs(targetPos.getY() - position.getY());
    return dist <= 1;
}

CellType Enemy::getCellType() const {
    return CellType::ENEMY;
}

Position Enemy::findBestMove(const Position& targetPos,
                            const vector<shared_ptr<Enemy>>& allEnemies,
                            const vector<shared_ptr<EnemyBuilding>>& allBuildings,
                            const vector<shared_ptr<AllyUnit>>& allAllies, // <-- добавлено
                            const GameField& field) {
    Position directions[4] = {Position(0,-1), Position(0,1), Position(-1,0), Position(1,0)};
    vector<Position> possibleMoves;
    vector<Position> goodMoves;
    random_device randomDevice;
    mt19937 generator(randomDevice());

    for (int i = 0; i < 4; i++) {
        Position newPosition(position.getX() + directions[i].getX(), position.getY() + directions[i].getY());

        if (!field.isValidPosition(newPosition) || field.isObstacleAt(newPosition.getX(), newPosition.getY())) {
            continue;
        }

        if (newPosition == targetPos) {
            continue;
        }

        bool collision = false;
        for (const auto& enemy : allEnemies) {
            if (enemy.get() != this && enemy->isAlive() && enemy->getPosition() == newPosition) {
                collision = true;
                break;
            }
        }

        if (collision) {
            continue;
        }

        for (const auto& building : allBuildings) {
            if (building->isAlive() && building->getPosition() == newPosition) {
                collision = true;
                break;
            }
        }

        if (collision) {
            continue;
        }

        for (const auto& ally : allAllies) {
            if (ally->isAlive() && ally->getPosition() == newPosition) {
                collision = true;
                break;
            }
        }

        if (collision) {
            continue;
        }

        possibleMoves.push_back(newPosition);

        int newDistance = std::abs(newPosition.getX() - targetPos.getX()) +
                          std::abs(newPosition.getY() - targetPos.getY());
        int currentDistance = std::abs(position.getX() - targetPos.getX()) +
                              std::abs(position.getY() - targetPos.getY());

        if (newDistance < currentDistance) {
            goodMoves.push_back(newPosition);
        }
    }

    if (!goodMoves.empty()) {
        uniform_int_distribution<int> distribution(0, goodMoves.size() - 1);
        return goodMoves[distribution(generator)];
    }

    if (!possibleMoves.empty()) {
        uniform_int_distribution<int> distribution(0, possibleMoves.size() - 1);
        return possibleMoves[distribution(generator)];
    }

    return Position(-1, -1);
}