#include "RangedAttackLogic.h"
#include "GameField.h"

bool RangedAttackLogic::isPathClear(int startX, int startY, int targetX, int targetY, const GameField& field) {
    int x = startX;
    int y = startY;
    int dx = (targetX > x) ? 1 : (targetX < x) ? -1 : 0;
    int dy = (targetY > y) ? 1 : (targetY < y) ? -1 : 0;

    x += dx;
    y += dy;
    while (x != targetX || y != targetY) {
        if (!field.isCellPassable(x, y)) {
            return false;
        }
        x += dx;
        y += dy;
    }
    return true;
}