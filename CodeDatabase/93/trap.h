#ifndef TRAP_H
#define TRAP_H

#include "game_entity.h"
#include <iostream>

class Trap {
private:
    Position position_;
    int damage_;
    bool active_;

public:
    Trap(int x = 0, int y = 0, int damage = 15);
    Position getPosition() const;
    int getDamage() const;
    bool isActive() const;
    void deactivate();

    void save(std::ostream& os) const;
    void load(std::istream& is);
};

#endif