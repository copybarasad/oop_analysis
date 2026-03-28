#include "Enemy.h"
#include "GameField.h"

void Enemy::Update(GameField& field) {
    auto [px, py] = field.PlayerCell();
    auto [ex, ey] = field.FindEntity(this);
    int dx = 0, dy = 0;
    if (ex < px) dx = 1; else if (ex > px) dx = -1;
    if (ey < py) dy = 1; else if (ey > py) dy = -1;
    if (dx != 0 || dy != 0)
        field.TryMove(this, dx, dy);
}

