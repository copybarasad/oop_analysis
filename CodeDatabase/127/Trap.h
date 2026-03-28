// Trap.h
#ifndef TRAP_H
#define TRAP_H

class Trap {
public:
    Trap(int x = 0, int y = 0, int damage = 0);
    int getX() const;
    int getY() const;
    int getDamage() const;
private:
    int x_;
    int y_;
    int damage_;
};

#endif 