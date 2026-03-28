#include "entity_manager.h"
#include <algorithm>

namespace rpg {
    void EntityManager::AddEnemy(int health, int damage, const Position &pos) {
        enemies_.emplace_back(health, damage, pos);
    }

    void EntityManager::AddBuilding(const Position &pos, int spawn_interval,
                                    int enemy_health, int enemy_damage) {
        buildings_.emplace_back(pos, spawn_interval, enemy_health, enemy_damage);
    }

    void EntityManager::AddTrap(const Position &pos, int damage) {
        traps_.emplace_back(pos, damage);
    }

    void EntityManager::AddAlly(int health, int damage, const Position &pos) {
        allies_.emplace_back(health, damage, pos);
    }

    void EntityManager::AddTower(const Position &pos) {
        towers_.emplace_back(pos);
    }

    void EntityManager::RemoveDeadEnemies() {
        enemies_.erase(
            std::remove_if(enemies_.begin(), enemies_.end(),
                           [](const Enemy &enemy) { return !enemy.IsAlive(); }),
            enemies_.end()
        );
    }

    void EntityManager::RemoveDeadAllies() {
        allies_.erase(
            std::remove_if(allies_.begin(), allies_.end(),
                           [](const Ally &ally) { return !ally.IsAlive(); }),
            allies_.end()
        );
    }

    [[nodiscard]] std::vector<Enemy> &EntityManager::GetEnemies() {
        return enemies_;
    }

    [[nodiscard]] const std::vector<Enemy> &EntityManager::GetEnemies() const {
        return enemies_;
    }

    [[nodiscard]] std::vector<EnemyBuilding> &EntityManager::GetBuildings() {
        return buildings_;
    }

    [[nodiscard]] const std::vector<EnemyBuilding> &EntityManager::GetBuildings() const {
        return buildings_;
    }

    [[nodiscard]] std::vector<Trap> &EntityManager::GetTraps() {
        return traps_;
    }

    [[nodiscard]] const std::vector<Trap> &EntityManager::GetTraps() const {
        return traps_;
    }

    [[nodiscard]] std::vector<Ally> &EntityManager::GetAllies() {
        return allies_;
    }

    [[nodiscard]] const std::vector<Ally> &EntityManager::GetAllies() const {
        return allies_;
    }

    [[nodiscard]] std::vector<EnemyTower> &EntityManager::GetTowers() {
        return towers_;
    }

    [[nodiscard]] const std::vector<EnemyTower> &EntityManager::GetTowers() const {
        return towers_;
    }

    [[nodiscard]] bool EntityManager::IsPositionOccupiedByEnemy(const Position &pos) const {
        for (const auto &enemy: enemies_) {
            if (enemy.GetPosition() == pos) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool EntityManager::IsPositionOccupiedByBuilding(const Position &pos) const {
        for (const auto &building: buildings_) {
            if (building.GetPosition() == pos) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] Enemy *EntityManager::FindEnemyAt(const Position &pos) {
        for (auto &enemy: enemies_) {
            if (enemy.GetPosition() == pos) {
                return &enemy;
            }
        }
        return nullptr;
    }
}
