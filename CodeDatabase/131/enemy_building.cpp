#include "enemy_building.h"
#include <algorithm>
#include <random>

namespace rpg {
    EnemyBuilding::EnemyBuilding(const Position &position, int spawn_interval,
                                 int enemy_health, int enemy_damage)
        : position_(position),
          spawn_interval_(std::max(1, spawn_interval)),
          enemy_health_(std::max(1, enemy_health)),
          enemy_damage_(std::max(0, enemy_damage)),
          turns_until_spawn_(spawn_interval_) {
    }
    
    EnemyBuilding EnemyBuilding::FromSave(const Position &position, int spawn_interval,
                                          int enemy_health, int enemy_damage, int turns_until_spawn) {
        EnemyBuilding building(position, spawn_interval, enemy_health, enemy_damage);
        building.turns_until_spawn_ = std::clamp(turns_until_spawn, 0, building.spawn_interval_);
        return building;
    }

    [[nodiscard]] Position EnemyBuilding::GetPosition() const {
        return position_;
    }

    [[nodiscard]] int EnemyBuilding::GetSpawnInterval() const {
        return spawn_interval_;
    }

    [[nodiscard]] int EnemyBuilding::GetEnemyHealth() const {
        return enemy_health_;
    }

    [[nodiscard]] int EnemyBuilding::GetEnemyDamage() const {
        return enemy_damage_;
    }

    [[nodiscard]] int EnemyBuilding::GetTurnsUntilSpawn() const {
        return turns_until_spawn_;
    }

    [[nodiscard]] std::optional<Enemy> EnemyBuilding::TrySpawn(
        const std::function<bool(Position)>& is_valid_position) {
        --turns_until_spawn_;

        if (turns_until_spawn_ > 0) {
            return std::nullopt;
        }

        turns_until_spawn_ = spawn_interval_;

        std::vector<Position> adjacent = {
            Position(position_.x, position_.y - 1),
            Position(position_.x, position_.y + 1),
            Position(position_.x - 1, position_.y),
            Position(position_.x + 1, position_.y)
        };

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::shuffle(adjacent.begin(), adjacent.end(), gen);

        for (const auto& pos : adjacent) {
            if (is_valid_position(pos)) {
                return Enemy(enemy_health_, enemy_damage_, pos);
            }
        }

        return std::nullopt;
    }
}
