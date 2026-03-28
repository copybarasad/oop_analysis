#include "Ally.h"
#include "GameField.h"
#include "Enemy.h"

void Ally::Update(GameField& field) {
    auto enemies = field.ListEnemies();
    if (enemies.empty()) return;

    auto target = enemies.front();
    auto [ax, ay] = field.FindEntity(this);
    int dx = 0, dy = 0;
    if (ax < target.first) dx = 1; else if (ax > target.first) dx = -1;
    if (ay < target.second) dy = 1; else if (ay > target.second) dy = -1;
    if (dx != 0 || dy != 0)
        field.TryMove(this, dx, dy);
}

