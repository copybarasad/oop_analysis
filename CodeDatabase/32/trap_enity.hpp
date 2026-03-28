#ifndef TRAP_ENTITY_HPP
#define TRAP_ENTITY_HPP

class Trap {
private:
    int x_;
    int y_;
    int damage_;
public:
    Trap(int x, int y, int damage) : x_(x), y_(y), damage_(damage) {}

    int GetX() const noexcept { return x_; }
    int GetY() const noexcept { return y_; }
    int GetDamage() const noexcept { return damage_; }
};

#endif