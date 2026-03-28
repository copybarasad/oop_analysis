#pragma once
#include <vector> 

#include "Entity.h"
#include "Enemy.h"

class Field;

class EnemyBuilding : public Entity {
public:
    EnemyBuilding(int x, int y, int enemy_health, int enemy_damage, int cooldown = 5, int max_enemies = 3);

    int GetSpawnCooldown() const;
    int GetCooldownCounter() const;
    int GetSpawnedEnemies() const;
    int GetMaxEnemies() const;

    void SetCooldownCounter(int cooldown_counter);
    void SetSpawnedEnemies(int spawned_enemies);

    bool IsEmpty() const;
    void Tick(Field& field, std::vector<Enemy>& enemies);
    void Attack(Entity& target) override;
    void TakeSpellDamage(int damage, bool can_slow = false) override;
    
    std::string GetDescription() const override;

private:
    int spawn_cooldown_;
    int cooldown_counter_;
    int max_enemies_;
    int spawned_enemies_;
    int enemy_health_;
    int enemy_damage_;
};
