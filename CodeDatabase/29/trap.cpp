#include "trap.hpp"
Trap::Trap(int x,int y,int dmg):x_(x),y_(y),damage_(dmg){}
int Trap::X() const noexcept { return x_; }
int Trap::Y() const noexcept { return y_; }
int Trap::Damage() const noexcept { return damage_; }