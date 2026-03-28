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

    int getX() const;

    int getY() const;

    int getDamage() const;

    bool isActive() const;

    void trigger();
};

#endif
