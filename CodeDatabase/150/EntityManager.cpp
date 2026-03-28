#include "EntityManager.hpp"
#include <algorithm>
#include <limits>

void EntityManager::updateEnemies(GameState& state) {
    for (auto& enemy : state.enemies) {
        if (enemy.isAlive()) {
            int playerX = state.player.getPositionX();
            int playerY = state.player.getPositionY();
            int enemyX = enemy.getPositionX();
            int enemyY = enemy.getPositionY();
            
            int distance = std::abs(playerX - enemyX) + std::abs(playerY - enemyY);
            if (distance <= 1) {
                state.player.takeDamage(enemy.getDamage());
                break;
            }
            
            Direction moveDir = Direction::None;
            if (std::abs(playerX - enemyX) > std::abs(playerY - enemyY)) {
                moveDir = (playerX > enemyX) ? Direction::Right : Direction::Left;
            } else {
                moveDir = (playerY > enemyY) ? Direction::Down : Direction::Up;
            }
            
            int newX = enemyX, newY = enemyY;
            switch (moveDir) {
                case Direction::Up: newY--; break;
                case Direction::Down: newY++; break;
                case Direction::Left: newX--; break;
                case Direction::Right: newX++; break;
                case Direction::None: break;
            }
            
            if (state.field.isValidPosition(newX, newY) && 
                state.field.getCell(newX, newY).isPassable() && 
                state.field.getCell(newX, newY).isEmpty()) {
                
                state.field.getCell(enemyX, enemyY).setHasEnemy(false);
                enemy.setPosition(newX, newY);
                state.field.getCell(newX, newY).setHasEnemy(true);
            }
        }
    }
    
    removeDeadEnemies(state);
}

void EntityManager::removeDeadEnemies(GameState& state) {
    for (auto it = state.enemies.begin(); it != state.enemies.end(); ) {
        if (!it->isAlive()) {
            int x = it->getPositionX();
            int y = it->getPositionY();
            
            if (state.field.isValidPosition(x, y)) {
                state.field.getCell(x, y).setHasEnemy(false);
            }
            
            state.player.addScore(10);
            state.player.gainMana(5);
            state.player.heal(5);
            
            it = state.enemies.erase(it);
        } else {
            ++it;
        }
    }
}

Enemy* EntityManager::getEnemyAt(GameState& state, int x, int y) {
    for (auto& enemy : state.enemies) {
        if (enemy.getPositionX() == x && enemy.getPositionY() == y && enemy.isAlive()) {
            return &enemy;
        }
    }
    return nullptr;
}

bool EntityManager::damageEnemyAt(GameState& state, int x, int y, int damage) {
    Enemy* enemy = getEnemyAt(state, x, y);
    if (enemy && enemy->isAlive()) {
        enemy->takeDamage(damage);
        
        if (!enemy->isAlive()) {
            state.field.getCell(x, y).setHasEnemy(false);
        }

        return true;
    }
    return false;
}

void EntityManager::updateAllies(GameState& state) {
    for (auto& ally : state.allies) {
        if (!ally.isAlive()) continue;
        
        int allyX = ally.getPositionX();
        int allyY = ally.getPositionY();
        
        Enemy* closestEnemy = nullptr;
        int minDistance = std::numeric_limits<int>::max();
        
        for (auto& enemy : state.enemies) {
            if (enemy.isAlive()) {
                int distance = std::abs(enemy.getPositionX() - allyX) + 
                              std::abs(enemy.getPositionY() - allyY);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestEnemy = &enemy;
                }
            }
        }
        
        if (!closestEnemy) continue;
        
        int targetX = closestEnemy->getPositionX();
        int targetY = closestEnemy->getPositionY();
        
        if (minDistance == 1) {
            damageEnemyAt(state, targetX, targetY, ally.getDamage());
            continue;
        }
        
        Direction moveDirection = Direction::None;
        if (std::abs(targetX - allyX) > std::abs(targetY - allyY)) {
            moveDirection = (targetX > allyX) ? Direction::Right : Direction::Left;
        } else {
            moveDirection = (targetY > allyY) ? Direction::Down : Direction::Up;
        }
        
        int newX = allyX;
        int newY = allyY;
        
        switch (moveDirection) {
            case Direction::Up: newY--; break;
            case Direction::Down: newY++; break;
            case Direction::Left: newX--; break;
            case Direction::Right: newX++; break;
            case Direction::None: break;
        }
        
        if (state.field.isValidPosition(newX, newY)) {
            const auto& targetCell = state.field.getCell(newX, newY);
            
            if (targetCell.isPassable() && targetCell.isEmpty()) {
                state.field.getCell(allyX, allyY).setHasAlly(false);
                ally.setPosition(newX, newY);
                state.field.getCell(newX, newY).setHasAlly(true);
            }
            else if (targetCell.hasEnemy()) {
                damageEnemyAt(state, newX, newY, ally.getDamage());
            }
        }
    }
    
    state.allies.erase(
        std::remove_if(state.allies.begin(), state.allies.end(),
            [](const Ally& ally) { return !ally.isAlive(); }),
        state.allies.end());
}

Ally* EntityManager::getAllyAt(GameState& state, int x, int y) {
    for (auto& ally : state.allies) {
        if (ally.getPositionX() == x && ally.getPositionY() == y && ally.isAlive()) {
            return &ally;
        }
    }
    return nullptr;
}

bool EntityManager::damageAllyAt(GameState& state, int x, int y, int damage) {
    if (Ally* ally = getAllyAt(state, x, y)) {
        ally->takeDamage(damage);
        return true;
    }
    return false;
}

void EntityManager::updateBuildings(GameState& state) {
    for (auto& building : state.buildings) {
        building->update();
        
        if (auto* tower = dynamic_cast<EnemyTower*>(building.get())) {
            if (tower->canAttack() && tower->isActive()) {
                tower->tryAttack(state.player.getPositionX(), state.player.getPositionY(), state);
            }
        }
        
        if (auto* enemyBuilding = dynamic_cast<EnemyBuilding*>(building.get())) {
            if (enemyBuilding->canSpawn() && enemyBuilding->isActive()) {
                int x = enemyBuilding->getPositionX();
                int y = enemyBuilding->getPositionY();
                
                std::vector<std::pair<int, int>> spawnPositions = {
                    {x-1, y}, {x+1, y}, {x, y-1}, {x, y+1}
                };
                
                for (const auto& pos : spawnPositions) {
                    if (state.field.isValidPosition(pos.first, pos.second) &&
                        state.field.getCell(pos.first, pos.second).isPassable() &&
                        state.field.getCell(pos.first, pos.second).isEmpty()) {
                        
                        Enemy newEnemy(pos.first, pos.second, 20 + state.currentLevel * 5, 5 + state.currentLevel);
                        state.enemies.push_back(newEnemy);
                        state.field.getCell(pos.first, pos.second).setHasEnemy(true);
                        enemyBuilding->resetCounter();
                        break;
                    }
                }
            }
        }
    }
    
    removeDestroyedBuildings(state);
}

void EntityManager::removeDestroyedBuildings(GameState& state) {
    for (int i = state.buildings.size() - 1; i >= 0; i--) {
        if (state.buildings[i]->isDestroyed()) {
            state.buildings.erase(state.buildings.begin() + i);
        }
    }
}

Building* EntityManager::getBuildingAt(GameState& state, int x, int y) {
    for (auto& building : state.buildings) {
        if (building->getPositionX() == x && 
            building->getPositionY() == y && 
            !building->isDestroyed()) {
            return building.get();
        }
    }
    return nullptr;
}

bool EntityManager::damageBuildingAt(GameState& state, int x, int y, int damage) {
    for (size_t i = 0; i < state.buildings.size(); i++) {
        auto& building = state.buildings[i];
        if (building->getPositionX() == x && 
            building->getPositionY() == y && 
            !building->isDestroyed()) {
            
            building->takeDamage(damage);
            
            if (building->isDestroyed()) {
                int reward = 25;
                
                if (dynamic_cast<EnemyTower*>(building.get())) {
                    reward = 50;
                } else if (dynamic_cast<EnemyBuilding*>(building.get())) {
                    reward = 35;
                }
                
                state.player.addScore(reward);
                state.player.restoreMana(15);
                state.player.heal(15);
                
                if (state.field.isValidPosition(x, y)) {
                    state.field.getCell(x, y).setHasBuilding(false);
                }
                
                return true;
            }
            return true;
        }
    }
    return false;
}

void EntityManager::updateTraps(GameState& state) {
    for (auto& trap : state.traps) {
        if (!trap.isActivated()) {
            for (auto& enemy : state.enemies) {
                if (enemy.getPositionX() == trap.getPositionX() && 
                    enemy.getPositionY() == trap.getPositionY() && 
                    enemy.isAlive()) {
                    damageEnemyAt(state, trap.getPositionX(), trap.getPositionY(), trap.getDamage());
                    trap.activate();
                    break;
                }
            }
        }
    }
    
    state.traps.erase(
        std::remove_if(state.traps.begin(), state.traps.end(),
            [](const Trap& trap) { return trap.isActivated(); }),
        state.traps.end());
}

void EntityManager::syncFieldWithEntities(GameState& state) {
    for (int y = 0; y < state.field.getHeight(); y++) {
        for (int x = 0; x < state.field.getWidth(); x++) {
            state.field.getCell(x, y).setHasPlayer(false);
            state.field.getCell(x, y).setHasEnemy(false);
            state.field.getCell(x, y).setHasAlly(false);
            state.field.getCell(x, y).setHasBuilding(false);
        }
    }
    
    state.field.getCell(state.player.getPositionX(), state.player.getPositionY())
        .setHasPlayer(true);
    
    for (const auto& enemy : state.enemies) {
        if (enemy.isAlive()) {
            state.field.getCell(enemy.getPositionX(), enemy.getPositionY())
                .setHasEnemy(true);
        }
    }
    
    for (const auto& ally : state.allies) {
        if (ally.isAlive()) {
            state.field.getCell(ally.getPositionX(), ally.getPositionY())
                .setHasAlly(true);
        }
    }
    
    for (const auto& building : state.buildings) {
        if (!building->isDestroyed()) {
            state.field.getCell(building->getPositionX(), building->getPositionY())
                .setHasBuilding(true);
        }
    }
}