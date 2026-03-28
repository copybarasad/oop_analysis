#ifndef UNIT_H
#define UNIT_H

class GameField; // forward
class Unit {
public:
    Unit(int x, int y, int hp, bool enemy);
    virtual ~Unit() = default;
    int x() const { return x_; }
    int y() const { return y_; }
    void setPosition(int x, int y) { x_ = x; y_ = y; }
    void takeDamage(int d);
    bool isDead() const { return hp_ <= 0; }
    int hp() const { return hp_; }
    bool isEnemy() const { return enemy_; }

    // Новое: действие юнита (по умолчанию ничего не делает).
    // Enemy переопределит этот метод.
    virtual void act(GameField& field) {}

private:
    int x_, y_;
    int hp_;
    bool enemy_;
};

#endif // UNIT_H
