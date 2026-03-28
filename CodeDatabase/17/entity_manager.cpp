#include "entity_manager.h"
#include <algorithm>
#include <iostream> 

EntityManager::EntityManager() {
    player = nullptr;
}

EntityManager::~EntityManager() {
    delete player;
    for (Enemy* enemy : enemies) delete enemy;
    for (EnemyBuilding* building : buildings) delete building;
    for (EnemyTower* tower : towers) delete tower;
    for (Follower* follower : allies) delete follower;
}

Player& EntityManager::getPlayer() { return *player; }
const Player& EntityManager::getPlayer() const { return *player; }

std::vector<Enemy*>& EntityManager::getEnemies() { return enemies; }
const std::vector<Enemy*>& EntityManager::getEnemies() const { return enemies; }

std::vector<EnemyBuilding*>& EntityManager::getBuildings() { return buildings; }
const std::vector<EnemyBuilding*>& EntityManager::getBuildings() const { return buildings; }

std::vector<EnemyTower*>& EntityManager::getTowers() { return towers; }
const std::vector<EnemyTower*>& EntityManager::getTowers() const { return towers; }

std::vector<Follower*>& EntityManager::getFollowers() {return allies;}
const std::vector<Follower*>& EntityManager::getFollowers() const {return allies;}



void EntityManager::createPlayer(int x, int y, int health) {
    if (player){
        player->getHand().removeHalfRandomly();
        player->update(x, y);
    }
    else{
        player = new Player(x, y, health);
    }
  
}

void EntityManager::entityNew(){
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    
    for (EnemyBuilding* building : buildings) {
        delete building;
    }
    buildings.clear();
    
    for (EnemyTower* tower : towers) {
        delete tower;
    }
    towers.clear();
    
    for (Follower* follower : allies) {
        delete follower;
    }
    allies.clear();
}

void EntityManager::removeDeadTower(GameMap& map) {
    towers.erase(
        std::remove_if(towers.begin(), towers.end(),
            [&map](EnemyTower* tower) {
                if (!tower->isAlive()) {
                    map.getCell(tower->getPosition()).setUsed(false);
                    delete tower;
                    return true;
                }
                return false;
            }),
        towers.end()
    );
}

void EntityManager::removeDeadEnemies(GameMap& map) {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [&map](Enemy* enemy) {
                if (!enemy->isAlive()) {
                    map.getCell(enemy->getPosition()).setUsed(false);
                    delete enemy;
                    return true;
                }
                return false;
            }),
        enemies.end()
    );
}

bool EntityManager::areAllEnemiesDead() const {
    for (Enemy* enemy : enemies) {
        if (enemy->isAlive()) {
            return false;
        }
    }
    return true;
}

void EntityManager::updateBuildings(GameMap& map) {
    for (EnemyBuilding* building : buildings) {
        building->spawn(enemies, map);
    }
}

void EntityManager::updateTower(GameMap& map) {
    for (EnemyTower* tower: towers) {
        if(!tower->isAlive()){
            map.getCell(tower->getPosition()).setEntity(nullptr);
            map.getCell(tower->getPosition()).setUsed(false);
        }
        tower->performAttack(map, player->getPosition());
    }
}

void EntityManager::updateEnemies(GameMap& map) {
    for (Enemy* enemy : enemies) {
        if (!enemy->isAlive() || enemy->shouldSkipTurn()) {
            if (enemy->shouldSkipTurn()) {
                enemy->setSkipTurn(false);
            }
            continue;
        }

        bool attacked = false;
        
        if (enemy->canAttack(player->getPosition())) {
            player->takeDamage(enemy->getDamage());
            attacked = true;
        } 
        else {
            for (Follower* ally : allies) {
                if (ally->isAlive() && enemy->canAttack(ally->getPosition())) {
                    ally->takeDamage(enemy->getDamage());
                    attacked = true;
                    
                    if (!ally->isAlive()) {
                        map.getCell(ally->getPosition()).setEntity(nullptr);
                        map.getCell(ally->getPosition()).setUsed(false);
                    }
                    break;
                }
            }
        }

        if (!attacked) {
            enemy->makeMove(map, player->getPosition());
        }

        if(!enemy->isAlive()){
            player->onEnemyKilled();
            map.getCell(enemy->getPosition()).setEntity(nullptr);
            map.getCell(enemy->getPosition()).setUsed(false);
        }
    }
}

void EntityManager::updateAllies(GameMap& map) {
    for (Follower* ally : allies) {
        if (!ally->isAlive()) continue;
        if (!ally->shouldSkipTurn()) {
            Enemy* closestEnemy = findClosestEnemy(ally->getPosition());
            if (closestEnemy && ally->canAttack(closestEnemy->getPosition())) {
                closestEnemy->takeDamage(ally->getDamage());
                if (!closestEnemy->isAlive()) {
                    map.getCell(closestEnemy->getPosition()).setEntity(nullptr);
                    map.getCell(closestEnemy->getPosition()).setUsed(false);
                    player->onEnemyKilled(); 
                }
            } else {
                Position target = closestEnemy ? closestEnemy->getPosition() : player->getPosition();
                ally->makeMove(map, target);
            }
        } else {
            ally->setSkipTurn(false);
        }
        if (!ally->isAlive()) {
            map.getCell(ally->getPosition()).setEntity(nullptr);
            map.getCell(ally->getPosition()).setUsed(false);
        }
    }
}

Enemy* EntityManager::findClosestEnemy(const Position& fromPos) {
    Enemy* closestEnemy = nullptr;
    
    for (Enemy* enemy : enemies) {
        if (!enemy->isAlive()) continue;
        
        if (!closestEnemy) {
            closestEnemy = enemy; 
        } else {
            int currentDist = std::abs(fromPos.getX() - enemy->getPosition().getX()) + 
                             std::abs(fromPos.getY() - enemy->getPosition().getY());
            int closestDist = std::abs(fromPos.getX() - closestEnemy->getPosition().getX()) + 
                             std::abs(fromPos.getY() - closestEnemy->getPosition().getY());
            
            if (currentDist < closestDist) {
                closestEnemy = enemy;
            }
        }
    }
    
    return closestEnemy;
}

void EntityManager::removeDeadAllies(GameMap& map) {
    allies.erase(
        std::remove_if(allies.begin(), allies.end(),
            [&map](Follower* ally) {
                if (!ally->isAlive()) {
                    map.getCell(ally->getPosition()).setUsed(false);
                    delete ally;
                    return true;
                }
                return false;
            }),
        allies.end()
    );
}

void EntityManager::movePlayer(int dx, int dy, GameMap& map) {
    if(player->shouldSkipTurn()){
        player->setSkipTurn(false);
    }
    else{
        if (!player->attack(dx, dy, map)) {
            player->move(dx, dy, map);
        }
    }
}

bool EntityManager::summonFollower(const Position& pos, GameMap& map) {
    if (!map.isPositionValid(pos)) return false;
    
    MapCell& cell = map.getCell(pos);
    if (cell.isUsed() || cell.getType() == MapCell::Type::WALL) return false;
    
    Follower* follower = new Follower(pos.getX(), pos.getY());
    allies.push_back(follower);
    
    cell.setEntity(follower);
    cell.setUsed(true);
    
    return true;
}
