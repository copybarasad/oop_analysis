#ifndef TRAP_H
#define TRAP_H

class Trap {
private:
    int damage;
    int positionX;
    int positionY;

public:
    Trap(int x, int y, int trapDamage = 30);
    
    int getDamage() const;
    int getPositionX() const;
    int getPositionY() const;
    
    bool isAt(int x, int y) const;
};

#endif // TRAP_H

