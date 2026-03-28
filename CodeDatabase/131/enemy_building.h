#ifndef OOP_ENEMY_BUILDING_H_
#define OOP_ENEMY_BUILDING_H_

#include "position.h"
#include "enemy.h"
#include <optional>
#include <functional>

namespace rpg {
    class EnemyBuilding final {
    public:
        EnemyBuilding(const Position &position, int spawn_interval,
                      int enemy_health, int enemy_damage);

        static EnemyBuilding FromSave(const Position &position, int spawn_interval,
                                      int enemy_health, int enemy_damage, int turns_until_spawn);

        [[nodiscard]] Position GetPosition() const;

        [[nodiscard]] int GetSpawnInterval() const;

        [[nodiscard]] int GetEnemyHealth() const;

        [[nodiscard]] int GetEnemyDamage() const;

        [[nodiscard]] int GetTurnsUntilSpawn() const;

        [[nodiscard]] std::optional<Enemy> TrySpawn(
            const std::function<bool(Position)>& is_valid_position);

    private:
        Position position_;
        int spawn_interval_;
        int enemy_health_;
        int enemy_damage_;
        int turns_until_spawn_;

        // Инвариант: spawn_interval_ > 0
        // Инвариант: turns_until_spawn_ >= 0 && turns_until_spawn_ <= spawn_interval_
        // Инвариант: enemy_health_ > 0
        // Инвариант: enemy_damage_ >= 0
    };
}

#endif
