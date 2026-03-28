#include "EnemyManager.h"
#include "BuildingManager.h"
#include "AllyManager.h"
#include "Ally.h"
#include "TowerManager.h"
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <queue>
#include <map>
#include <set>

EnemyManager::EnemyManager()
    : rng_(static_cast<unsigned int>(
          std::chrono::system_clock::now().time_since_epoch().count()))
{
}

void EnemyManager::addEnemy(std::unique_ptr<Enemy> enemy, int x, int y) {
    if (enemy) {
        enemy->setPosition(x, y);
        enemies_.push_back(std::move(enemy));
    }
}

int EnemyManager::getEnemyCount() const {
    return static_cast<int>(enemies_.size());
}

void EnemyManager::removeDeadEnemies() {
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return !enemy->isAlive();
            }),
        enemies_.end()
    );
}

std::vector<Enemy*> EnemyManager::getEnemies() const {
    std::vector<Enemy*> result;
    for (const auto& enemy : enemies_) {
        if (enemy && enemy->isAlive()) {
            result.push_back(enemy.get());
        }
    }
    return result;
}

void EnemyManager::processEnemyMovement(Player& player, const BuildingManager& buildingManager, AllyManager* allyManager, TowerManager* towerManager, const std::vector<std::vector<Cell>>& grid, int width, int height) {
    if (!player.isAlive()) return;

    Pos playerPos = player.getPosition();

    for (auto& enemy : enemies_) {
        if (!enemy || !enemy->isAlive()) continue;
        if (enemy->shouldSkipTurn()) {
            enemy->resetSkipTurn();
            continue;
        }

        Pos enemyPos = enemy->getPosition();
        

        std::vector<Pos> path = findPathBFS(enemyPos, playerPos, player, buildingManager, 
                                            allyManager, towerManager, grid, width, height);

        int nx = enemyPos.x;
        int ny = enemyPos.y;


        if (!path.empty()) {
            nx = path[0].x;
            ny = path[0].y;
        }


        if (nx == playerPos.x && ny == playerPos.y) {

            player.takeDamage(enemy->getDamage());
            std::cout << "Enemy at (" << enemyPos.x << "," << enemyPos.y << ") attacks player for " 
                      << enemy->getDamage() << " damage! Player HP: " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
        } else if (allyManager && allyManager->hasAllyOnCell(nx, ny)) {

            Ally& ally = allyManager->getAllyAt(nx, ny);
            ally.takeDamage(enemy->getDamage());
            std::cout << "Enemy at (" << enemyPos.x << "," << enemyPos.y << ") attacks ally at (" 
                      << nx << "," << ny << ") for " << enemy->getDamage() << " damage! Ally HP: " 
                      << ally.getHealth() << "/" << ally.getMaxHealth() << "\n";
        } else if (nx != enemyPos.x || ny != enemyPos.y) {

            enemy->setPosition(nx, ny);


            if (isValidPosition(nx, ny, width, height) && grid[ny][nx].getType() == CellType::SLOW) {
                enemy->skipNextMove();
            }
        }
    }
}

bool EnemyManager::hasEnemyOnCell(int x, int y) const {
    for (const auto& enemy : enemies_) {
        if (enemy && enemy->isAlive()) {
            Pos pos = enemy->getPosition();
            if (pos.x == x && pos.y == y) {
                return true;
            }
        }
    }
    return false;
}

Enemy& EnemyManager::getEnemyAt(int x, int y) const {
    for (const auto& enemy : enemies_) {
        if (enemy && enemy->isAlive()) {
            Pos pos = enemy->getPosition();
            if (pos.x == x && pos.y == y) {
                return *enemy.get();
            }
        }
    }
    throw std::runtime_error("Enemy not found at position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
}

bool EnemyManager::isValidPosition(int x, int y, int width, int height) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool EnemyManager::isCellPassable(const Cell& cell) const {
    return cell.isPassable();
}

bool EnemyManager::isCellEmpty(int x, int y, const Player& player, const BuildingManager& buildingManager, AllyManager* allyManager, TowerManager* towerManager, const std::vector<std::vector<Cell>>& grid, int width, int height) const {
    if (player.getPosition().x == x && player.getPosition().y == y) {
        return false;
    }

    if (hasEnemyOnCell(x, y)) {
        return false;
    }
    
    if (buildingManager.hasBuildingOnCell(x, y)) {
        return false;
    }
    
    if (allyManager && allyManager->hasAllyOnCell(x, y)) {
        return false;
    }
    
    if (towerManager && towerManager->hasTowerOnCell(x, y)) {
        return false;
    }
    
    return true;
}


struct PosComparator {
    bool operator()(const Pos& a, const Pos& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

std::vector<Pos> EnemyManager::findPathBFS(Pos start, Pos target, const Player& player, 
                                           const BuildingManager& buildingManager,
                                           AllyManager* allyManager, TowerManager* towerManager,
                                           const std::vector<std::vector<Cell>>& grid, 
                                           int width, int height) const {
    std::vector<Pos> path;
    

    int dist = std::abs(start.x - target.x) + std::abs(start.y - target.y);
    if (dist <= 1) {
        if (dist == 1) path.push_back(target);
        return path;
    }
    

    std::queue<Pos> queue;
    std::set<Pos, PosComparator> visited;
    std::map<Pos, Pos, PosComparator> parent;
    
    queue.push(start);
    visited.insert(start);
    

    std::vector<Pos> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    
    bool found = false;
    
    while (!queue.empty() && !found) {
        Pos current = queue.front();
        queue.pop();
        

        if (current.x == target.x && current.y == target.y) {
            found = true;
            break;
        }
        

        for (const auto& dir : directions) {
            Pos next = {current.x + dir.x, current.y + dir.y};
            

            if (visited.find(next) != visited.end()) {
                continue;
            }
            

            if (!isValidPosition(next.x, next.y, width, height) || 
                !isCellPassable(grid[next.y][next.x])) {
                continue;
            }
            

            if (next.x == target.x && next.y == target.y) {
                parent[next] = current;
                visited.insert(next);
                queue.push(next);
                found = true;
                break;
            }
            

            if (!isCellEmpty(next.x, next.y, player, buildingManager, allyManager, towerManager, grid, width, height)) {
                continue;
            }
            

            parent[next] = current;
            visited.insert(next);
            queue.push(next);
        }
    }
    

    if (found) {
        Pos current = target;
        while (parent.find(current) != parent.end() && !(parent[current].x == start.x && parent[current].y == start.y)) {
            path.push_back(current);
            current = parent[current];
        }
        if (!path.empty()) {
            path.push_back(current);
        }
        std::reverse(path.begin(), path.end());
    }
    
    return path;
}
