#ifndef TRAP_H
#define TRAP_H

class Trap {
private:
    int positionX;
    int positionY;
    int damage;
    bool active;

public:
    Trap(int x, int y, int trapDamage);
    
    int getPositionX() const;
    int getPositionY() const;
    int getDamage() const;
    bool isActive() const;
    void trigger();
};

#endif