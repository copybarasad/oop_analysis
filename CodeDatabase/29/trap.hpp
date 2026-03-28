#ifndef TRAP_HPP
#define TRAP_HPP
class Trap {
    int x_;
    int y_;
    int damage_;
public:
    Trap(int x,int y,int dmg);
    int X() const noexcept;
    int Y() const noexcept;
    int Damage() const noexcept;
};
#endif