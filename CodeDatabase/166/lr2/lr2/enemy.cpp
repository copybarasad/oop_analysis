#include "enemy.hpp"

// двигаемся на одну клетку в сторону цели, по горизонтали или вертикали
Position Enemy::chooseStep(const Position& from, const Position& to) {
    Position step{0, 0};

    // считаем направление по x
    if (from.getX() < to.getX()) step.setX(1);
    else if (from.getX() > to.getX()) step.setX(-1);

    // считаем направление по y
    if (from.getY() < to.getY()) step.setY(1);
    else if (from.getY() > to.getY()) step.setY(-1);

    // если получился диагональный ход, выбираем одно направление рандомно
    if (step.getX() != 0 && step.getY() != 0) {
        if (std::rand() % 2 == 0) step.setX(0);
        else step.setY(0);
    }
    return step;
}
