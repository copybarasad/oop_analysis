#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class Game_field;
class Player;

class Enemy{
private:
    std::string name;
    int health;
    int max_health;
    int damage;
    bool is_alive;
    Game_field* field;

public:
    Enemy(const std::string& name, Game_field* field);
    std::string get_name() const;
    int get_health() const;
    int get_damage() const;
    bool get_is_alive() const;

    void set_health(int new_health);

    void take_damage(int damage_amount);
    bool make_move(Player& player);
    void display_status() const;  
};

#endif