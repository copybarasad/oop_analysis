#ifndef TRAP_H
#define TRAP_H

class Trap {
public:
    Trap(int x, int y, int damage);
    int getX() const;
    int getY() const;
    int getDamage() const;
private:
    int x, y, damage;
};

#endif