#ifndef ENEMY_H
#define ENEMY_H

class Player;
class Playing_field;  

class Enemy{
private:
    int max_life;
    int current_life;
    int damage;
    int position_x;
    int position_y;
    bool is_alive;

public:
    Enemy(int start_x, int start_y);

    void move(int player_x, int player_y, Player& player, const Playing_field& field);  

    void taking_damage(int damage);
    bool attack_player(Player& player);

    void position(int new_x, int new_y);

    bool get_alive() const;
    int get_position_x() const;
    int get_position_y() const;
    int get_curr_life() const;
    int get_damage() const;

    void increase_health(int bonus_health);
    void increase_damage(int bonus_damage);
    void set_health(int new_health);
    void set_damage(int new_damage);
    void set_alive(bool alive);
};

#endif