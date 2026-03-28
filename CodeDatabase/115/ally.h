#ifndef ALLY_H
#define ALLY_H

#include "playing_field.h"
#include <vector>

class Enemy;
class Player; 

class Ally {
private:
    int position_x;
    int position_y;
    int health;
    int damage;
    bool alive;

public:
    Ally(int x, int y, int unit_health = 1, int unit_damage = 1);

    void move_independently(const Playing_field& field, std::vector<Enemy>& enemies, Player& player);
    
    void attack_enemy(Enemy& enemy, Player& player);
    
    void take_damage(int damage_amount);
    int get_position_x() const;
    int get_position_y() const;
    bool is_alive() const;
};

#endif