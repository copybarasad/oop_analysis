#pragma once

class Field;

class Ally {
private:
    int x;
    int y;
    int health;
    int damage;
    int speed;
public:
    Ally(int pos_x, int pos_y, int hp = 10, int dmg = 15);

    void update(Field& field);
    void take_damage(int dmg);

    int get_x() const;
    int get_y() const;
    bool is_alive() const;

    char get_symbol() const;
};