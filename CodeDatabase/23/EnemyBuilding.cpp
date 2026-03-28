#include "EnemyBuilding.h"
#include "Enemy.h"

EnemyBuilding::EnemyBuilding(int x, int y, const std::string& enemy_spawn_name, 
                             int enemy_hp, int enemy_dmg, int spawn_cooldown, int hp)
    : Entity(x, y, hp), enemy_spawn_name(enemy_spawn_name), enemy_hp(enemy_hp), 
      enemy_dmg(enemy_dmg), spawn_cooldown(spawn_cooldown), current_cooldown(0) {}

std::string EnemyBuilding::getEnemySpawnName() const {
    return enemy_spawn_name;
}

int EnemyBuilding::getEnemyHp() const {
    return enemy_hp;
}

int EnemyBuilding::getEnemyDamage() const {
    return enemy_dmg;
}

int EnemyBuilding::getSpawnCooldown() const {
    return spawn_cooldown;
}

int EnemyBuilding::getCurrentCooldown() const {
    return current_cooldown;
}

bool EnemyBuilding::tick() {
    if (current_cooldown > 0) {
        current_cooldown--;
        return false;
    }
    return true;
}

Enemy* EnemyBuilding::spawnEnemy() {
    if (current_cooldown == 0) {
        current_cooldown = spawn_cooldown;
        return new Enemy(x, y, enemy_spawn_name, enemy_hp, enemy_dmg);
    }
    return nullptr;
}

 
