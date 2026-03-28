#include "EnemyBuilding.h"
#include "Field.h"
#include <iostream>

constexpr int kBuildingHealth = 10;

EnemyBuilding::EnemyBuilding(int x, int y, int enemy_health, int enemy_damage,
                             int cooldown, int max_enemies)
    : Entity(kBuildingHealth, 0, x, y),
      spawn_cooldown_(cooldown),
      cooldown_counter_(1),
      max_enemies_(max_enemies),
      spawned_enemies_(0),
      enemy_health_(enemy_health), 
      enemy_damage_(enemy_damage) {}

int EnemyBuilding::GetSpawnCooldown() const { 
    return spawn_cooldown_;
}

int EnemyBuilding::GetCooldownCounter() const {
    return cooldown_counter_;
}

int EnemyBuilding::GetSpawnedEnemies() const {
    return spawned_enemies_;
}

int EnemyBuilding::GetMaxEnemies() const {
    return max_enemies_;
}

void EnemyBuilding::SetCooldownCounter(int cooldown_counter) {
    cooldown_counter_ = cooldown_counter;
}

void EnemyBuilding::SetSpawnedEnemies(int spawned_enemies) {
    spawned_enemies_ = spawned_enemies;
}

bool EnemyBuilding::IsEmpty() const {
    return spawned_enemies_ >= max_enemies_;
}

void EnemyBuilding::Tick(Field& field, std::vector<Enemy>& enemies) {
    if (spawned_enemies_ >= max_enemies_) {
        return;
    }

    cooldown_counter_--;
    if (cooldown_counter_ == 0) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int nx = x_ + dx;
                int ny = y_ + dy;

                if (field.IsCellEmpty(nx, ny)) {
                    enemies.emplace_back(enemy_health_, enemy_damage_, nx, ny);
                    field.SetCellType(nx, ny, CellType::ENEMY);
                    spawned_enemies_++;
                    cooldown_counter_ = spawn_cooldown_;
                    return;
                }
            }
        }
        cooldown_counter_ = 1;
    }
}

void EnemyBuilding::Attack(Entity&) {
}

void EnemyBuilding::TakeSpellDamage(int damage, bool) {
    TakeDamage(damage);
    std::cout << "Вражеское здание получило " << damage << " единицы урона!\n";
}

std::string EnemyBuilding::GetDescription() const {
    return "Вражеское здание: HP = " + std::to_string(GetHealth())
           + ", pos = (" + std::to_string(GetX()) + "," + std::to_string(GetY()) + ")";
}