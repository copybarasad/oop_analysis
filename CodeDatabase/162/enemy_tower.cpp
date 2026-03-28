#include "enemy_tower.h"
#include "field.h"
#include "player.h"

EnemyTower::EnemyTower(int x, int y, int range, int damage) : x_(x), y_(y), range_(range), damage_(damage) {}

void EnemyTower::Update(Field& field, Player& player) {
    if (cooldown_ > 0) {
        cooldown_--;
        return;
    }
    int dist = std::abs(x_ - field.GetPlayerX()) + std::abs(y_ - field.GetPlayerY());
    if (dist <= range_ && !IsDestroyed()) { 
        player.TakeDamage(damage_);
        cooldown_ = 2;
    }
}