#ifndef TRAP_H
#define TRAP_H

#include "Point.h"
#include <iostream>

class Trap {
public:
    Trap(int x, int y, int damage);

    Point getPosition() const;
    int getDamage() const;

    void save(std::ostream& os) const;
    void load(std::istream& is);

private:
    Point position_;
    int damage_;
};

#endif