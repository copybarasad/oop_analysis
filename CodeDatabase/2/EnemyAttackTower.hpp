#pragma once
#include <vector>
class Player;
class Ally;

class EnemyAttackTower {
private:
    int x, y;
    int damage;
    int hp;
    int countdown;
    int radius;
    int level;
public:
    EnemyAttackTower(int cur_x, int cur_y, int start_level);

    void get_coords(int& tower_x, int& tower_y) const;
    int get_damage() const;
    int get_hp() const;
    int get_countdown() const;
    void setLevel(int new_level);

    void attack_player(Player& player);
    void attack_ally(Ally& ally);
    void attack_in_range(Player& player, std::vector<Ally>& allies);
    void take_damage(int player_damage);
    bool is_death() const;
    void set_hp(int new_hp) { hp = new_hp; }
    void set_position(int new_x,int new_y) ;
};