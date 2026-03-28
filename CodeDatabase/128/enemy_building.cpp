#include "enemy_building.hpp"
#include "gamemodel.hpp"
#include "field.hpp"
#include "constants.hpp"
#include <iostream>
#include "enemy.hpp"
#include <cstdlib>
#include <iostream>

EnemyBuilding::EnemyBuilding(int x, int y, int delay)
    : EnemyStructure(x, y, configs::kBaseHealth, configs::kBaseDamage, delay) {}

void EnemyBuilding::Tick(Game& game) {
    if (!isAlive()) return;
    if (++counter_ < delay_) return;
    counter_ = 0;
    auto& field = game.getField();
    auto& enemies = game.getEnemies();
    int& next_id = game.getNextEnemyId();
    static const int dx8[8] = { 1,-1,0,0,1,1,-1,-1 };
    static const int dy8[8] = { 0,0,1,-1,1,-1,1,-1 };
    for (int i = 0; i < 8; ++i) {
        int nx = x_ + dx8[i];
        int ny = y_ + dy8[i];
        if (!game.InBounds(nx, ny)) continue;
        auto& cell = field.At(nx, ny);
        if (cell.getType() == CellType::kEmpty) {
            int health = 15 + rand() % 10;
            int damage = 2 + rand() % 4;
            enemies.push_back({ next_id++, Enemy(health, damage), nx, ny });
            field.At(nx, ny).set(CellType::kEnemy, enemies.back().id);
            break;
        }
    }
}
