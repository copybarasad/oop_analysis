#include "enemy.hpp"

// двигаемся на одну клетку в сторону цели, по горизонтали или вертикали
Position Enemy::chooseStep(const Position& from, const Position& to) {
    Position step{0, 0};

    // считаем направление по x
    if (from.x < to.x) step.x = 1;
    else if (from.x > to.x) step.x = -1;

    // считаем направление по y
    if (from.y < to.y) step.y = 1;
    else if (from.y > to.y) step.y = -1;

    // если получился диагональный ход, выбираем одно направление рандомно
    if (step.x != 0 && step.y != 0) {
        if (std::rand() % 2 == 0) step.x = 0;
        else step.y = 0;
    }
    return step;
}
