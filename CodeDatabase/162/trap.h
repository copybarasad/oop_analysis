#ifndef TRAP_H_
#define TRAP_H_

class Trap {
public:
    Trap(int x, int y, int damage);
    int GetX() const { return x_; }
    int GetY() const { return y_; }
    int GetDamage() const { return damage_; }

private:
    int x_, y_, damage_;
};

#endif // TRAP_H_

