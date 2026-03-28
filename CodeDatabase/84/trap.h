#ifndef TRAP_H
#define TRAP_H

class Trap {
private:
    int position_x_;
    int position_y_;
    int damage_;

public:
    Trap(int x, int y, int damage);
    
    int get_x() const;
    int get_y() const;
    int get_damage() const;
    bool is_at_position(int x, int y) const;
};

#endif