#ifndef TRAPS_HPP_INCLUDED
#define TRAPS_HPP_INCLUDED

#include "position.hpp"


class Trap {
private:
    Position pos_;
    int damage_{0};
public:
    Trap() = default;
    Trap(Position p, int dmg): pos_(p), damage_(dmg) {}
    const Position& getPos() const noexcept { return pos_; }
    void setPos(const Position& p) noexcept { pos_ = p; }
    int getDamage() const noexcept { return damage_; }
    void setDamage(int v) noexcept { damage_ = v; }
};


#endif // TRAPS_HPP_INCLUDED
