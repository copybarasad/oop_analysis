#include "EntityManager.h"
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <sstream>

EntityManager::EntityManager() {}

void EntityManager::createPlayer(int x, int y, int health, int melee, int ranged) {
    player_ = std::make_unique<Player>(x, y, health, melee, ranged);
}

void EntityManager::addEnemy(std::unique_ptr<Enemy> enemy) { enemies_.push_back(std::move(enemy)); }
void EntityManager::addBuilding(std::unique_ptr<EnemyBuilding> building) { buildings_.push_back(std::move(building)); }
void EntityManager::addTower(std::unique_ptr<EnemyTower> tower) { towers_.push_back(std::move(tower)); }
void EntityManager::addTrap(std::unique_ptr<Trap> trap) { traps_.push_back(std::move(trap)); }

void EntityManager::addAlly(std::unique_ptr<Entity> entity) {
    if (auto ally = std::unique_ptr<Ally>(dynamic_cast<Ally*>(entity.release()))) {
        allies_.push_back(std::move(ally));
    } else if (auto enemy = std::unique_ptr<Enemy>(dynamic_cast<Enemy*>(entity.release()))) {
         enemies_.push_back(std::move(enemy));
    }
}

void EntityManager::updateAll(Game& game) {
    for (auto& enemy : enemies_) enemy->update(game);
    for (auto& ally : allies_) ally->update(game);
    for (auto& building : buildings_) building->update(game);
    for (auto& tower : towers_) tower->update(game);
}

void EntityManager::removeDeadEntities() {
    auto remove_dead = [](auto& container) {
        container.erase(std::remove_if(container.begin(), container.end(), 
            [](const auto& entity) { return !entity->isAlive(); }), 
            container.end());
    };
    remove_dead(enemies_);
    remove_dead(buildings_);
    remove_dead(allies_);
    remove_dead(towers_);
}

void EntityManager::checkTrapsFor(Game& game, Entity* entity) {
    if (!entity) return;
    auto it = traps_.begin();
    while (it != traps_.end()) {
        if ((*it)->getPosition() == entity->getPosition()) {
            bool is_enemy = dynamic_cast<Enemy*>(entity) != nullptr;
            bool is_player = dynamic_cast<Player*>(entity) != nullptr;
            if (is_enemy || is_player) {
                int damage = (*it)->getDamage();
                
                std::stringstream ss;
                ss << (is_player ? "You" : "An enemy") << " stepped on a trap and took " << damage << " damage!";
                game.notifyLog(ss.str());
                
                if (entity->takeDamage(damage) && is_enemy) {
                    game.notifyLog("The enemy was defeated by the trap!");
                    if(player_) player_->addScore(10);
                }

                it = traps_.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
}

void EntityManager::clearNonPlayerEntities() {
    enemies_.clear();
    buildings_.clear();
    towers_.clear();
    traps_.clear();
    allies_.clear();
}

void EntityManager::clearAll() {
    clearNonPlayerEntities();
    player_.reset();
}

Player* EntityManager::getPlayer() const { return player_.get(); }
const std::vector<std::unique_ptr<Enemy>>& EntityManager::getEnemies() const { return enemies_; }
const std::vector<std::unique_ptr<Ally>>& EntityManager::getAllies() const { return allies_; }
const std::vector<std::unique_ptr<EnemyBuilding>>& EntityManager::getBuildings() const { return buildings_; }
const std::vector<std::unique_ptr<EnemyTower>>& EntityManager::getTowers() const { return towers_; }
const std::vector<std::unique_ptr<Trap>>& EntityManager::getTraps() const { return traps_; }

Entity* EntityManager::getEntityAt(const Point& pos) const {
    if(player_ && player_->getPosition() == pos) return player_.get();
    for(const auto& e : enemies_) if(e->getPosition() == pos) return e.get();
    for(const auto& b : buildings_) if(b->getPosition() == pos) return b.get();
    for(const auto& a : allies_) if(a->getPosition() == pos) return a.get();
    for(const auto& t : towers_) if(t->getPosition() == pos) return t.get();
    return nullptr;
}

Trap* EntityManager::getTrapAt(const Point& pos) const {
    for(const auto& t : traps_) if(t->getPosition() == pos) return t.get();
    return nullptr;
}