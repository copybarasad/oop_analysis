#include "AISystem.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

void AISystem::processEnemiesTurn(std::vector<Enemy>& enemies, Player& player, GameField& field) {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            processEnemyBehavior(enemy, player, field);
        }
    }
}

void AISystem::spawnEnemyFromBuilding(EnemyBuilding& building, std::vector<Enemy>& enemies, GameField& field) {
    if (!building.isAlive()) {
        return;
    }
    
    if (!building.canSpawnEnemy()) {
        return;
    }
    
    auto spawnPositions = building.getSpawnPositions(field);
    if (spawnPositions.empty()) {
        return;
    }
    
    auto spawnPos = selectSpawnPosition(spawnPositions);
    if (field.placeObjectAtPosition(spawnPos)) {
        enemies.emplace_back(spawnPos);
        building.spawnEnemy();
    }
}

void AISystem::processEnemyBehavior(Enemy& enemy, Player& player, GameField& field) {
    if (checkJockerTrap(enemy, field)) {
        return;
    }
    
    if (shouldAttackPlayer(enemy, player)) {
        executeAttack(enemy, player);
        return;
    }
    
    tryMovement(enemy, player, field);
}

bool AISystem::checkJockerTrap(Enemy& enemy, GameField& field) {
    auto enemyPos = enemy.getPosition();
    
    if (!field.isPositionValid(enemyPos)) {
        return false;
    }
    
    const Cell& cell = field.getCell(enemyPos);
    if (cell.getType() == CellType::JOCKER) {
        int trapDamage = 20;
        enemy.takeDamage(trapDamage);
        field.getCell(enemyPos).setType(CellType::NORMAL);
        
        // Если враг умер от ловушки - убираем его
        if (!enemy.isAlive()) {
            field.removeObjectFromPosition(enemyPos);
        }
        return true;
    }
    
    return false;
}

void AISystem::executeAttack(Enemy& enemy, Player& player) {
    if (enemy.canAttackPlayer(player)) {
        enemy.attackPlayer(player);
    }
}

bool AISystem::tryMovement(Enemy& enemy, const Player& player, GameField& field) {
    auto direction = calculateMovementDirection(enemy, player);
    auto enemyPos = enemy.getPosition();
    
    Position primaryPos = {
        enemyPos.first + direction.first,
        enemyPos.second + direction.second
    };
    
    if (isValidMovementPosition(primaryPos, field)) {
        return performMovement(enemy, primaryPos, field);
    }
    
    return tryAlternativeMovements(enemy, player, field);
}

bool AISystem::performMovement(Enemy& enemy, const Position& newPos, GameField& field) {
    auto oldPos = enemy.getPosition();
    
    field.removeObjectFromPosition(oldPos);
    
    int deltaX = newPos.first - oldPos.first;
    int deltaY = newPos.second - oldPos.second;
    enemy.move(deltaX, deltaY);
    
    bool placed = field.placeObjectAtPosition(newPos);
    if (!placed) {
        enemy.setPosition(oldPos);
        field.placeObjectAtPosition(oldPos);
        return false;
    }
    
    return true;
}

bool AISystem::tryAlternativeMovements(Enemy& enemy, const Player& player, GameField& field) {
    auto enemyPos = enemy.getPosition();
    
    std::vector<Position> directions = {
        {1, 0},   
        {0, 1},   
        {-1, 0},  
        {0, -1}   
    };
    
    std::sort(directions.begin(), directions.end(), 
        [&](const Position& a, const Position& b) {
            Position posA = {
                enemyPos.first + a.first, 
                enemyPos.second + a.second
            };
            Position posB = {
                enemyPos.first + b.first, 
                enemyPos.second + b.second
            };
            double distA = PositionUtils::calculateDistance(posA, player.getPosition());
            double distB = PositionUtils::calculateDistance(posB, player.getPosition());
            return distA < distB;
        });
    
    for (const auto& direction : directions) {
        Position newPos = {
            enemyPos.first + direction.first,
            enemyPos.second + direction.second
        };
        
        if (isValidMovementPosition(newPos, field)) {
            return performMovement(enemy, newPos, field);
        }
    }
    
    return false;
}

bool AISystem::shouldAttackPlayer(const Enemy& enemy, const Player& player) const {
    if (!player.isAlive()) {
        return false;
    }
    
    double distance = enemy.distanceTo(player);
    return distance <= 1.5;
}

Position AISystem::calculateMovementDirection(const Enemy& enemy, const Player& player) const {
    auto playerPos = player.getPosition();
    auto enemyPos = enemy.getPosition();
    
    int diffX = playerPos.first - enemyPos.first;
    int diffY = playerPos.second - enemyPos.second;
    
    if (std::abs(diffX) > std::abs(diffY)) {
        return { (diffX > 0) ? 1 : -1, 0 };
    } else {
        return { 0, (diffY > 0) ? 1 : -1 };
    }
}

bool AISystem::isValidMovementPosition(const Position& newPos, const GameField& field) const {
    if (!field.isPositionValid(newPos)) {
        return false;
    }
    
    return field.canMoveToPosition(newPos);
}

Position AISystem::selectSpawnPosition(const std::vector<Position>& positions) const {
    if (positions.empty()) {
        return Position(0, 0);
    }
    
    int randomValue = std::rand() % positions.size();
    return positions[randomValue];
}