#ifndef TRAP_H
#define TRAP_H

class Game_field;

class Trap{
private:
    int x;
    int y;
    int damage;
    bool active;

public:
    Trap(int pos_x, int pos_y, int trap_damage);
    int get_x() const;
    int get_y() const;
    int get_damage() const;
    bool is_active() const;
    void trigger();
    void update(Game_field& field);
};

#endif