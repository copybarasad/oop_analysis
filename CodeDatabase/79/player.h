#ifndef PLAYER_H
#define PLAYER_H
#include "basic_info.h"
#include "spell_hand.h"
#include <vector>
#include <cstdlib>
#include <memory>

class Enemy;
class Tower;
class Player: public BasicInfo{
private:
    std::string name;
    int bow_damage;
    int sword_damage;
    int weapon;
    SpellHand spellHand;

public:
    Player(std::string name_, int maxHealth_ = 25, int health_ = 20, int points_ = 10, int damage_ = 5, int x_ = 2, int y_ = 2);
    
    Player(Player&&) = default;
    Player& operator=(Player&&) = default;

    int use_spell(int num, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
    int buy_spell();

    void increase_points(int value);

    int change_weapon();

    int attack(Enemy& enemy);
    int attack(Tower& tower);

    void print_current_stats();

    void move(int x_, int y_);
    void getInfo();
    int remove_random_spell();
    void heal();
    void increase_max_health(int n);
    void increase_damage(int n);
    ~Player();
    std::vector<int> get_spell_types() const;
    void load_spells(const std::vector<int>& spell_types);
    int upgrade_spell(int value);
    void get_spell_info();
    void to_start();
};
#endif