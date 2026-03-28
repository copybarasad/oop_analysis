#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "board.hpp"
#include <stdexcept>

class Actor {
public:
    Actor(int hp, int damage, Point pos);
    virtual ~Actor();

    int HP() const noexcept;
    int BaseDamage() const noexcept;
    Point Pos() const noexcept;

    void SetPos(Point p) noexcept;
    bool IsAlive() const noexcept;
    void TakeDamage(int dmg);

protected:
    int hp_;
    int base_damage_;
    Point pos_;
};

#endif