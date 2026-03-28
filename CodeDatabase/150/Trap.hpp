#ifndef TRAP_H
#define TRAP_H

#include <iostream>

class Trap {
private:
    int positionX_;
    int positionY_;
    int damage_;
    bool activated_;

public:
    Trap();
    Trap(int x, int y, int damage = 25);
    
    void activate();

    void serialize(std::ostream& os) const;
    void deserialize(std::istream& is);
    
    int getPositionX() const { return positionX_; }
    int getPositionY() const { return positionY_; }
    int getDamage() const { return damage_; }
    bool isActivated() const { return activated_; }
    
    void setPositionX(int x) { positionX_ = x; }
    void setPositionY(int y) { positionY_ = y; }
    void setDamage(int damage) { damage_ = damage; }
    void setActivated(bool activated) { activated_ = activated; }
};

#endif