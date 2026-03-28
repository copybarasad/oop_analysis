#include "GameField.h"
#include "GameConstants.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <string>
#include <algorithm>
#include <random>

namespace {
    std::vector<Position> getAdjacent(const Position& p) {
        return {Position(p.x - 1, p.y), Position(p.x + 1, p.y), Position(p.x, p.y - 1), Position(p.x, p.y + 1)};
    }
}

GameField::GameField() 
    : width(0), height(0) {
}

GameField::GameField(int fieldWidth, int fieldHeight) 
    : width(fieldWidth), height(fieldHeight) {

    if (width < Constants::MIN_FIELD_SIZE || width > Constants::MAX_FIELD_SIZE ||
        height < Constants::MIN_FIELD_SIZE || height > Constants::MAX_FIELD_SIZE) {
        throw std::invalid_argument("Invalid field size");
    }

    grid.resize(width);
    for (int x = 0; x < width; ++x) {
        grid[x].resize(height);
        for (int y = 0; y < height; ++y) {
            grid[x][y] = Cell(x, y);
        }
    }
}

void GameField::spawnObstacles(double obstacleCoeff) {
    int area = width * height;
    int maxObstacles = static_cast<int>(area * obstacleCoeff);

    if (maxObstacles < Constants::MIN_OBSTACLES) maxObstacles = Constants::MIN_OBSTACLES;
    if (maxObstacles > Constants::MAX_OBSTACLES) maxObstacles = Constants::MAX_OBSTACLES;

    int obstaclesPlaced = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Position> freeCells;
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            freeCells.emplace_back(x, y);
    std::shuffle(freeCells.begin(), freeCells.end(), gen);

    for (const auto& pos : freeCells) {
        if (obstaclesPlaced >= maxObstacles)
            break;
        if (placeObstacle(pos))
            ++obstaclesPlaced;
    }
}

bool GameField::isStartValid(const Position& pos) const {
    for (const auto& adj : getAdjacent(pos)) {
        if (isValidPosition(adj) && isPositionPassable(adj) && isPositionEmpty(adj)) {
            return true;
        }
    }
    return false;
}

void GameField::ensurePlayerHasFreeNeighbor() {
    if (!isStartValid(playerPosition)) {
        for (const auto& adj : getAdjacent(playerPosition)) {
            if (isValidPosition(adj)) {
                if (grid[adj.x][adj.y].getType() == CellType::OBSTACLE) {
                    grid[adj.x][adj.y].setType(CellType::EMPTY);
                }
            }
        }
    }
}

bool GameField::tryGetRandomEmptyPosition(Position& outPos, int maxAttempts) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, width - 1);
    std::uniform_int_distribution<> disY(0, height - 1);

    for (int attempts = 0; attempts < maxAttempts; ++attempts) {
        Position pos(disX(gen), disY(gen));
        if (isPositionEmpty(pos)) {
            outPos = pos;
            return true;
        }
    }

    std::vector<Position> all;
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            if (isPositionEmpty(Position(x, y)))
                all.emplace_back(x, y);
    if (!all.empty()) {
        outPos = all.front();
        return true;
    }
    return false;
}

bool GameField::placePlayer(const Position& pos) {
    if (!isValidPosition(pos) || !isPositionPassable(pos) || !isPositionEmpty(pos)) {
        return false;
    }
    playerPosition = pos;
    grid[pos.x][pos.y].setType(CellType::PLAYER);
    return true;
}

bool GameField::placeEnemy(const Position& pos, Enemy enemy) {
    if (!isValidPosition(pos) || !isPositionPassable(pos) || !isPositionEmpty(pos)) {
        return false;
    }
    enemies.emplace_back(pos, enemy);
    grid[pos.x][pos.y].setType(CellType::ENEMY);
    return true;
}

bool GameField::placeObstacle(const Position& pos) {
    if (!isValidPosition(pos) || !isPositionEmpty(pos)) {
        return false;
    }
    grid[pos.x][pos.y].setType(CellType::OBSTACLE);
    return true;
}

bool GameField::isValidPosition(const Position& pos) const {
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

bool GameField::isPositionEmpty(const Position& pos) const {
    if (!isValidPosition(pos)) return false;
    return grid[pos.x][pos.y].getType() == CellType::EMPTY;
}

bool GameField::isPositionPassable(const Position& pos) const {
    if (!isValidPosition(pos)) return false;
    return grid[pos.x][pos.y].isPassable();
}

bool GameField::checkIsValidPosition(const Position& pos) const {
    return isValidPosition(pos);
}

Position GameField::getRandomEmptyPosition() const {
    Position pos;
    if (!tryGetRandomEmptyPosition(pos, width * height)) {
        throw std::runtime_error("No empty position available");
    }
    return pos;
}

GameField::GameField(const GameField& other) 
    : grid(other.grid),
      width(other.width), 
      height(other.height),
      playerPosition(other.playerPosition),
      enemies(other.enemies),
      currentTurnLog(other.currentTurnLog),
      previousTurnLog(other.previousTurnLog) {}

GameField::GameField(GameField&& other) noexcept 
    : grid(std::move(other.grid)),
      width(other.width), 
      height(other.height),
      playerPosition(std::move(other.playerPosition)),
      enemies(std::move(other.enemies)),
      currentTurnLog(std::move(other.currentTurnLog)),
      previousTurnLog(std::move(other.previousTurnLog)) {
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        grid = other.grid;
        width = other.width;
        height = other.height;
        playerPosition = other.playerPosition;
        enemies = other.enemies;
        currentTurnLog = other.currentTurnLog;
        previousTurnLog = other.previousTurnLog;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        grid = std::move(other.grid);
        width = other.width;
        height = other.height;
        playerPosition = std::move(other.playerPosition);
        enemies = std::move(other.enemies);
        currentTurnLog = std::move(other.currentTurnLog);
        previousTurnLog = std::move(other.previousTurnLog);

        other.width = 0;
        other.height = 0;
    }
    return *this;
}

int GameField::calculateDistance(const Position& a, const Position& b) const {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

bool GameField::isPlayerDetected(const Position& enemyPos) const {
    int distanceToPlayer = calculateDistance(enemyPos, playerPosition);
    return distanceToPlayer <= Constants::ENEMY_DETECTION_RANGE;
}

Direction GameField::getRandomDirection() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    
    Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    return directions[dis(gen)];
}

Direction GameField::getDirectionTowardsPlayer(const Position& enemyPos) const {
    int dx = playerPosition.x - enemyPos.x;
    int dy = playerPosition.y - enemyPos.y;

    if (std::abs(dx) > std::abs(dy)) {
        return (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    } else {
        return (dy > 0) ? Direction::DOWN : Direction::UP;
    }
}

Direction GameField::findBestMoveToPlayer(const Position& enemyPos) const {
    int dx = playerPosition.x - enemyPos.x;
    int dy = playerPosition.y - enemyPos.y;

    Direction primaryDir = Direction::NONE;
    Direction secondaryDir = Direction::NONE;

    if (std::abs(dx) > std::abs(dy)) {
        primaryDir = (dx > 0) ? Direction::RIGHT : Direction::LEFT;
        secondaryDir = (dy > 0) ? Direction::DOWN : Direction::UP;
    } else {
        primaryDir = (dy > 0) ? Direction::DOWN : Direction::UP;
        secondaryDir = (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    }

    Position primaryOffset = DirectionHelper::getPositionOffset(primaryDir);
    Position primaryNewPos = enemyPos + primaryOffset;

    if (isValidPosition(primaryNewPos)) {
        CellType cellType = grid[primaryNewPos.x][primaryNewPos.y].getType();
        
        if (cellType != CellType::PLAYER && cellType != CellType::OBSTACLE) {
            bool hasEnemy = false;
            for (const auto& ep : enemies) {
                if (ep.first == primaryNewPos && ep.second.isAlive()) {
                    hasEnemy = true;
                    break;
                }
            }
            
            bool hasTower = false;
            for (const auto& t : towers) {
                if (t.getPosition() == primaryNewPos && t.isAlive()) {
                    hasTower = true;
                    break;
                }
            }
            
            if (!hasEnemy && !hasTower) {
                return primaryDir;
            }
        }
    }

    Position secondaryOffset = DirectionHelper::getPositionOffset(secondaryDir);
    Position secondaryNewPos = enemyPos + secondaryOffset;

    if (isValidPosition(secondaryNewPos)) {
        CellType cellType = grid[secondaryNewPos.x][secondaryNewPos.y].getType();
        
        if (cellType != CellType::PLAYER && cellType != CellType::OBSTACLE) {
            bool hasEnemy = false;
            for (const auto& ep : enemies) {
                if (ep.first == secondaryNewPos && ep.second.isAlive()) {
                    hasEnemy = true;
                    break;
                }
            }
            
            bool hasTower = false;
            for (const auto& t : towers) {
                if (t.getPosition() == secondaryNewPos && t.isAlive()) {
                    hasTower = true;
                    break;
                }
            }
            
            if (!hasEnemy && !hasTower) {
                return secondaryDir;
            }
        }
    }

    Direction allDirections[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    for (Direction dir : allDirections) {
        if (dir == primaryDir || dir == secondaryDir) continue;

        Position offset = DirectionHelper::getPositionOffset(dir);
        Position newPos = enemyPos + offset;

        if (isValidPosition(newPos)) {
            CellType cellType = grid[newPos.x][newPos.y].getType();
            
            if (cellType != CellType::PLAYER && cellType != CellType::OBSTACLE) {
                bool hasEnemy = false;
                for (const auto& ep : enemies) {
                    if (ep.first == newPos && ep.second.isAlive()) {
                        hasEnemy = true;
                        break;
                    }
                }
                
                bool hasTower = false;
                for (const auto& t : towers) {
                    if (t.getPosition() == newPos && t.isAlive()) {
                        hasTower = true;
                        break;
                    }
                }
                
                if (!hasEnemy && !hasTower) {
                    return dir;
                }
            }
        }
    }

    return Direction::NONE;
}

bool GameField::movePlayer(Direction direction, Player& player) {
    if (direction == Direction::NONE) {
        return true;
    }

    Position offset = DirectionHelper::getPositionOffset(direction);
    Position newPos = playerPosition + offset;

    if (!isValidPosition(newPos)) {
        return false;
    }

    for (auto& enemyPair : enemies) {
        if (enemyPair.first == newPos && enemyPair.second.isAlive()) {
            if (player.getAttackType() == AttackType::MELEE) {
                int damage = player.getDamage();
                enemyPair.second.takeDamage(damage);

                addToCurrentTurnLog("Player melee attacked enemy at (" + 
                                  std::to_string(newPos.x) + ", " + std::to_string(newPos.y) + 
                                  ") for " + std::to_string(damage) + " damage.");

                if (enemyPair.second.isAlive()) {
                    addToCurrentTurnLog("Enemy health: " + std::to_string(enemyPair.second.getHealth()));
                } else {
                    addToCurrentTurnLog("Enemy defeated.");
                    player.incrementEnemyKill();
                }

                if (!enemyPair.second.isAlive()) {
                    removeDeadEnemies();
                }
                return true;
            } else {
                addToCurrentTurnLog("Cannot melee attack in ranged mode. Switch to melee first.");
                return false;
            }
        }
    }
    
    for (auto& tower : towers) {
        if (tower.getPosition() == newPos && tower.isAlive()) {
            if (player.getAttackType() == AttackType::MELEE) {
                int damage = player.getDamage();
                tower.takeDamage(damage);

                addToCurrentTurnLog("Player melee attacked tower at (" + 
                                  std::to_string(newPos.x) + ", " + std::to_string(newPos.y) + 
                                  ") for " + std::to_string(damage) + " damage.");

                if (tower.isAlive()) {
                    addToCurrentTurnLog("Tower health: " + std::to_string(tower.getHealth()));
                } else {
                    addToCurrentTurnLog("Tower destroyed.");
                    player.incrementEnemyKill();
                }

                if (!tower.isAlive()) {
                    removeDeadTowers();
                }
                return true;
            } else {
                addToCurrentTurnLog("Cannot melee attack in ranged mode. Switch to melee first.");
                return false;
            }
        }
    }

    if (!isPositionPassable(newPos)) {
        return false;
    }

    grid[playerPosition.x][playerPosition.y].setType(CellType::EMPTY);
    grid[newPos.x][newPos.y].setType(CellType::PLAYER);
    playerPosition = newPos;
    return true;
}

bool GameField::performRangedAttack(Direction direction, Player& player) {
    if (direction == Direction::NONE) {
        return false;
    }

    if (player.getAttackType() != AttackType::RANGED) {
        addToCurrentTurnLog("Ranged attack requires switching to ranged mode first.");
        return false;
    }

    Position offset = DirectionHelper::getPositionOffset(direction);
    Position checkPos = playerPosition;

    for (int i = 0; i < Constants::RANGED_ATTACK_RANGE; ++i) {
        checkPos = checkPos + offset;

        if (!isValidPosition(checkPos)) {
            break;
        }

        if (!isPositionPassable(checkPos) && grid[checkPos.x][checkPos.y].getType() != CellType::ENEMY && 
            grid[checkPos.x][checkPos.y].getType() != CellType::TOWER) {
            break;
        }

        for (auto& enemyPair : enemies) {
            if (enemyPair.first == checkPos && enemyPair.second.isAlive()) {
                int damage = player.getDamage();
                enemyPair.second.takeDamage(damage);

                addToCurrentTurnLog("Player ranged attacked enemy at (" + 
                                  std::to_string(checkPos.x) + ", " + std::to_string(checkPos.y) + 
                                  ") for " + std::to_string(damage) + " damage.");

                if (enemyPair.second.isAlive()) {
                    addToCurrentTurnLog("Enemy health: " + std::to_string(enemyPair.second.getHealth()));
                } else {
                    addToCurrentTurnLog("Enemy defeated.");
                    player.incrementEnemyKill();
                }

                if (!enemyPair.second.isAlive()) {
                    removeDeadEnemies();
                }
                return true;
            }
        }

        for (auto& tower : towers) {
            if (tower.getPosition() == checkPos && tower.isAlive()) {
                int damage = player.getDamage();
                tower.takeDamage(damage);

                addToCurrentTurnLog("Player ranged attacked tower at (" + 
                                  std::to_string(checkPos.x) + ", " + std::to_string(checkPos.y) + 
                                  ") for " + std::to_string(damage) + " damage.");

                if (tower.isAlive()) {
                    addToCurrentTurnLog("Tower health: " + std::to_string(tower.getHealth()));
                } else {
                    addToCurrentTurnLog("Tower destroyed.");
                    player.incrementEnemyKill();
                }

                if (!tower.isAlive()) {
                    removeDeadTowers();
                }
                return true;
            }
        }
    }

    addToCurrentTurnLog("Ranged attack missed.");
    return true;
}

void GameField::moveEnemies(Player& player) {
    for (auto& enemyPair : enemies) {
        Position currentPos = enemyPair.first;
        Enemy& enemy = enemyPair.second;

        if (!enemy.isAlive()) continue;

        for (auto& trap : traps) {
            if (trap.isActive() && trap.getPosition() == currentPos) {
                enemy.takeDamage(trap.getDamage());
                trap.trigger();
                addToCurrentTurnLog("Enemy stepped on trap! " + std::to_string(trap.getDamage()) + " damage!");
                grid[currentPos.x][currentPos.y].setType(CellType::EMPTY);
                if (!enemy.isAlive()) {
                    addToCurrentTurnLog("Enemy defeated by trap!");
                    player.incrementEnemyKill();
                }
                break;
            }
        }

        if (!enemy.isAlive()) continue;

        int distanceToPlayer = calculateDistance(currentPos, playerPosition);

        if (distanceToPlayer == 1) {
            int damage = enemy.getDamage();
            player.takeDamage(damage);
            addToCurrentTurnLog("Enemy at (" + std::to_string(currentPos.x) + ", " + 
                              std::to_string(currentPos.y) + ") attacked player for " + 
                              std::to_string(damage) + " damage.");
            addToCurrentTurnLog("Player health: " + std::to_string(player.getHealth()));
        } else if (isPlayerDetected(currentPos)) {
            Direction moveDirection = findBestMoveToPlayer(currentPos);

            if (moveDirection != Direction::NONE) {
                Position offset = DirectionHelper::getPositionOffset(moveDirection);
                Position newPos = currentPos + offset;

                if (!isValidPosition(newPos)) continue;

                CellType cellType = grid[newPos.x][newPos.y].getType();
                
                if (cellType == CellType::PLAYER || cellType == CellType::OBSTACLE) {
                    continue;
                }

                bool hasEnemy = false;
                for (const auto& ep : enemies) {
                    if (ep.first == newPos && ep.second.isAlive()) {
                        hasEnemy = true;
                        break;
                    }
                }

                bool hasTower = false;
                for (const auto& t : towers) {
                    if (t.getPosition() == newPos && t.isAlive()) {
                        hasTower = true;
                        break;
                    }
                }

                if (!hasEnemy && !hasTower) {
                    grid[currentPos.x][currentPos.y].setType(CellType::EMPTY);
                    grid[newPos.x][newPos.y].setType(CellType::ENEMY);
                    enemyPair.first = newPos;
                }
            }
        } else {
            Direction moveDirection = getRandomDirection();

            if (moveDirection != Direction::NONE) {
                Position offset = DirectionHelper::getPositionOffset(moveDirection);
                Position newPos = currentPos + offset;

                if (!isValidPosition(newPos)) continue;

                CellType cellType = grid[newPos.x][newPos.y].getType();
                
                if (cellType == CellType::PLAYER || cellType == CellType::OBSTACLE) {
                    continue;
                }

                bool hasEnemy = false;
                for (const auto& ep : enemies) {
                    if (ep.first == newPos && ep.second.isAlive()) {
                        hasEnemy = true;
                        break;
                    }
                }

                bool hasTower = false;
                for (const auto& t : towers) {
                    if (t.getPosition() == newPos && t.isAlive()) {
                        hasTower = true;
                        break;
                    }
                }

                if (!hasEnemy && !hasTower) {
                    grid[currentPos.x][currentPos.y].setType(CellType::EMPTY);
                    grid[newPos.x][newPos.y].setType(CellType::ENEMY);
                    enemyPair.first = newPos;
                }
            }
        }
    }

    removeDeadEnemies();
    removeDeadTraps();
}

void GameField::removeDeadEnemies() {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!it->second.isAlive()) {
            grid[it->first.x][it->first.y].setType(CellType::EMPTY);
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<Position> GameField::getAreaDamagePositions(const Position& targetPos, 
                                                         Direction direction, int areaSize) const {
    std::vector<Position> positions;
    
    Position topLeft;
    
    if (direction == Direction::LEFT) {
        topLeft = Position(targetPos.x, targetPos.y - (areaSize - 1));
    } 
    else if (direction == Direction::RIGHT) {
        topLeft = Position(targetPos.x - (areaSize - 1), targetPos.y);
    } 
    else if (direction == Direction::UP) {
        topLeft = targetPos;
    } 
    else if (direction == Direction::DOWN) {
        topLeft = Position(targetPos.x - (areaSize - 1), targetPos.y - (areaSize - 1));
    }
    
    for (int x = topLeft.x; x < topLeft.x + areaSize; ++x) {
        for (int y = topLeft.y; y < topLeft.y + areaSize; ++y) {
            Position pos(x, y);
            if (checkIsValidPosition(pos)) {
                positions.push_back(pos);
            }
        }
    }
    
    return positions;
}

bool GameField::damageEnemyAt(const Position& pos, int damage) {
    if (!checkIsValidPosition(pos)) {
        return false;
    }
    
    for (auto& enemyPair : enemies) {
        if (enemyPair.first == pos && enemyPair.second.isAlive()) {
            enemyPair.second.takeDamage(damage);
            if (!enemyPair.second.isAlive()) {
                removeDeadEnemies();
            }
            return true;
        }
    }
    return false;
}

void GameField::damageEnemiesInArea(const std::vector<Position>& positions, int damage) {
    for (const auto& pos : positions) {
        damageEnemyAt(pos, damage);
    }
}

void GameField::endTurn() {
    previousTurnLog = currentTurnLog;
    currentTurnLog.clear();
}

void GameField::addToCurrentTurnLog(const std::string& message) {
    currentTurnLog.push_back(message);
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

Cell GameField::getCell(const Position& pos) const {
    if (!isValidPosition(pos)) {
        throw std::out_of_range("Position out of field bounds");
    }
    return grid[pos.x][pos.y];
}

Position GameField::getPlayerPosition() const {
    return playerPosition;
}

std::vector<Position> GameField::getEnemyPositions() const {
    std::vector<Position> positions;
    for (const auto& enemyPair : enemies) {
        if (enemyPair.second.isAlive()) {
            positions.push_back(enemyPair.first);
        }
    }
    return positions;
}

int GameField::getEnemyCount() const {
    int count = 0;
    for (const auto& enemyPair : enemies) {
        if (enemyPair.second.isAlive()) {
            count++;
        }
    }
    return count;
}

const std::vector<std::string>& GameField::getPreviousTurnLog() const {
    return previousTurnLog;
}

bool GameField::isPositionBlockedByObstacle(const Position& pos) const {
    if (!checkIsValidPosition(pos)) {
        return true;
    }
    return !isPositionPassable(pos);
}

std::vector<Trap>& GameField::getTraps() {
    return traps;
}

const std::vector<Trap>& GameField::getTraps() const {
    return traps;
}

bool GameField::placeTrap(const Position& pos, int damage) {
    if (!checkIsValidPosition(pos)) {
        return false;
    }
    
    if (!isPositionEmpty(pos)) {
        return false;
    }
    
    traps.emplace_back(pos, damage);
    grid[pos.x][pos.y].setType(CellType::TRAP);
    return true;
}

void GameField::removeDeadTraps() {
    for (auto it = traps.begin(); it != traps.end(); ) {
        if (!it->isActive()) {
            grid[it->getPosition().x][it->getPosition().y].setType(CellType::EMPTY);
            it = traps.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<EnemyTower>& GameField::getTowers() {
    return towers;
}

const std::vector<EnemyTower>& GameField::getTowers() const {
    return towers;
}

bool GameField::addTower(const Position& pos, EnemyTower&& tower) {
    if (!checkIsValidPosition(pos) || !isPositionPassable(pos) || !isPositionEmpty(pos)) {
        return false;
    }
    
    towers.push_back(std::move(tower));
    grid[pos.x][pos.y].setType(CellType::TOWER);
    return true;
}


int GameField::getTowerCount() const {
    int count = 0;
    for (const auto& tower : towers) {
        if (tower.isAlive()) {
            count++;
        }
    }
    return count;
}

void GameField::removeDeadTowers() {
    for (auto it = towers.begin(); it != towers.end(); ) {
        if (!it->isAlive()) {
            grid[it->getPosition().x][it->getPosition().y].setType(CellType::EMPTY);
            it = towers.erase(it);
        } else {
            ++it;
        }
    }
}

int GameField::checkTowerAttackOnPlayer(const Position& playerPos) {
    int totalDamage = 0;
    
    for (auto& tower : towers) {
        if (!tower.isAlive()) continue;
        
        int distance = calculateDistance(tower.getPosition(), playerPos);
        
        if (distance <= tower.getAttackRange() && tower.canAttackThisTurn()) {
            totalDamage += tower.getDamage();
            addToCurrentTurnLog("Enemy Tower at (" + std::to_string(tower.getPosition().x) + ", " + 
                            std::to_string(tower.getPosition().y) + ") attacked! " + 
                            std::to_string(tower.getDamage()) + " damage!");
            tower.resetAttackCooldown();
        } else {
            tower.decreaseCooldown();
        }
    }
    
    removeDeadTowers();
    return totalDamage;
}

std::vector<Position> GameField::getTowerPositions() const {
    std::vector<Position> positions;
    for (const auto& tower : towers) {
        if (tower.isAlive()) {
            positions.push_back(tower.getPosition());
        }
    }
    return positions;
}

bool GameField::damageTowerAt(const Position& pos, int damage) {
    if (!checkIsValidPosition(pos)) {
        return false;
    }
    
    for (auto& tower : towers) {
        if (tower.getPosition() == pos && tower.isAlive()) {
            tower.takeDamage(damage);
            
            addToCurrentTurnLog("Tower at (" + std::to_string(pos.x) + ", " + 
                              std::to_string(pos.y) + ") took " + std::to_string(damage) + " damage!");
            
            if (!tower.isAlive()) {
                addToCurrentTurnLog("Tower destroyed!");
                removeDeadTowers();
            } else {
                addToCurrentTurnLog("Tower health: " + std::to_string(tower.getHealth()));
            }
            
            return true;
        }
    }
    return false;
}

bool GameField::damageTargetInLine(const Position& startPos, Direction direction, int range, 
                                   int damage) {
    Position offset = DirectionHelper::getPositionOffset(direction);
    Position checkPos = startPos;
    
    for (int i = 0; i < range; ++i) {
        checkPos = checkPos + offset;
        
        if (!checkIsValidPosition(checkPos)) break;
        
        CellType cellType = grid[checkPos.x][checkPos.y].getType();
        if (cellType == CellType::OBSTACLE) break;
        
        for (auto& enemyPair : enemies) {
            if (enemyPair.first == checkPos && enemyPair.second.isAlive()) {
                enemyPair.second.takeDamage(damage);
                
                addToCurrentTurnLog("Enemy at (" + std::to_string(checkPos.x) + ", " + 
                                  std::to_string(checkPos.y) + ") took " + 
                                  std::to_string(damage) + " damage!");
                
                if (!enemyPair.second.isAlive()) {
                    addToCurrentTurnLog("Enemy defeated.");
                    removeDeadEnemies();
                }
                return true;
            }
        }
        
        for (auto& tower : towers) {
            if (tower.getPosition() == checkPos && tower.isAlive()) {
                tower.takeDamage(damage);
                
                addToCurrentTurnLog("Tower at (" + std::to_string(checkPos.x) + ", " + 
                                  std::to_string(checkPos.y) + ") took " + std::to_string(damage) + " damage!");
                
                if (!tower.isAlive()) {
                    addToCurrentTurnLog("Tower destroyed.");
                    removeDeadTowers();
                } else {
                    addToCurrentTurnLog("Tower health: " + std::to_string(tower.getHealth()));
                }
                return true;
            }
        }
    }
    
    return false;
}

bool GameField::hasTrapAt(const Position& pos) const {
    for (const auto& trap : traps) {
        if (trap.getPosition() == pos && trap.isActive()) {
            return true;
        }
    }
    return false;
}

bool GameField::addTowerWithState(const Position& pos, EnemyTower&& tower) {
    if (!checkIsValidPosition(pos) || !isPositionPassable(pos) || !isPositionEmpty(pos)) {
        return false;
    }
    
    towers.push_back(std::move(tower));
    grid[pos.x][pos.y].setType(CellType::TOWER);
    return true;
}

Enemy* GameField::getEnemyAt(const Position& pos) {
    for (auto& enemyPair : enemies) {
        if (enemyPair.first == pos && enemyPair.second.isAlive()) {
            return &enemyPair.second;
        }
    }
    return nullptr;
}

const Enemy* GameField::getEnemyAt(const Position& pos) const {
    for (const auto& enemyPair : enemies) {
        if (enemyPair.first == pos && enemyPair.second.isAlive()) {
            return &enemyPair.second;
        }
    }
    return nullptr;
}