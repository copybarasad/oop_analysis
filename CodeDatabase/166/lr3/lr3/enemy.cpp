#include "enemy.hpp"

// двигаемся на одну клетку в сторону цели, по горизонтали или вертикали
Position Enemy::chooseStep(const Position& from, const Position& to) {
    int sx=0, sy=0;

    // считаем направление по x
    if (from.getX() < to.getX()) sx = 1;
    else if (from.getX() > to.getX()) sx = -1;

    // считаем направление по y
    if (from.getY() < to.getY()) sy = 1;
    else if (from.getY() > to.getY()) sy = -1;

    // если получился диагональный ход, выбираем одно направление рандомно
    if (sx != 0 && sy != 0) {
        if (std::rand() % 2 == 0) sx = 0;
        else sy = 0;
    }
    return Position(sx, sy);
}
