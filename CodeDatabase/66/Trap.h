#ifndef TRAP_H
#define TRAP_H

class Trap {
private:
    int damage;

public:
    Trap(int damage);
    
    int getDamage() const;
};

#endif
