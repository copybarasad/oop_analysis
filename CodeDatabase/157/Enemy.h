#ifndef ENEMY_H
#define ENEMY_H

#include "Unit.h"

class GameField;
class Player;

class Enemy : public Unit {
public:
    // x,y,hp, damage
    Enemy(int x, int y, int hp, int damage);
    virtual ~Enemy() = default;

    int damage() const { return damage_; }

    // переопределяем поведение: пытаемся шагнуть к игроку на 1 клетку.
    // Если цель — клетка игрока, наносим урон игроку и не перемещаемся.
    void act(GameField& field) override;

private:
    int damage_;
};

#endif // ENEMY_H
