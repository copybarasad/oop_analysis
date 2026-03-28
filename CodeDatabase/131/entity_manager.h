#ifndef OOP_ENTITY_MANAGER_H_
#define OOP_ENTITY_MANAGER_H_

#include "enemy.h"
#include "enemy_building.h"
#include "trap.h"
#include "ally.h"
#include "enemy_tower.h"
#include "position.h"
#include <vector>

namespace rpg {
    class EntityManager final {
    public:
        EntityManager() = default;

        void AddEnemy(int health, int damage, const Position &pos);

        void AddBuilding(const Position &pos, int spawn_interval,
                         int enemy_health, int enemy_damage);

        void AddTrap(const Position &pos, int damage);

        void AddAlly(int health, int damage, const Position &pos);

        void AddTower(const Position &pos);

        void RemoveDeadEnemies();

        void RemoveDeadAllies();

        [[nodiscard]] std::vector<Enemy> &GetEnemies();

        [[nodiscard]] const std::vector<Enemy> &GetEnemies() const;

        [[nodiscard]] std::vector<EnemyBuilding> &GetBuildings();

        [[nodiscard]] const std::vector<EnemyBuilding> &GetBuildings() const;

        [[nodiscard]] std::vector<Trap> &GetTraps();

        [[nodiscard]] const std::vector<Trap> &GetTraps() const;

        [[nodiscard]] std::vector<Ally> &GetAllies();

        [[nodiscard]] const std::vector<Ally> &GetAllies() const;

        [[nodiscard]] std::vector<EnemyTower> &GetTowers();

        [[nodiscard]] const std::vector<EnemyTower> &GetTowers() const;

        [[nodiscard]] bool IsPositionOccupiedByEnemy(const Position &pos) const;

        [[nodiscard]] bool IsPositionOccupiedByBuilding(const Position &pos) const;

        [[nodiscard]] Enemy *FindEnemyAt(const Position &pos);

    private:
        std::vector<Enemy> enemies_;
        std::vector<EnemyBuilding> buildings_;
        std::vector<Trap> traps_;
        std::vector<Ally> allies_;
        std::vector<EnemyTower> towers_;
    };
}

#endif
