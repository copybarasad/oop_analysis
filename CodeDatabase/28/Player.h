#pragma once
#include "Entity.h"
#include "Cell.h"
#include "Hand.h"

class Player : public Entity {
private:
    enum class Weapon { Melee, Range };
    Weapon current_weapon;
    int damage;
    int melee_damage = 25;
    int range_damage = 150;
    bool can_move = true;
    bool slowed = false;
    int max_hp;
    int score;
    std::unique_ptr<Hand> hand;

public:
    Player(int hp, int x, int y, size_t hand_capacity);

    void switch_weapon();
    void start_turn();
    void go_to(Cell& cell) override;
    void take_damage(int dmg);
    void edit_score(int value);
    void edit_max_hp(int hp);
    void heal(int amount);

    Hand* get_hand() const { return hand.get(); }
    int get_score() const;
    int get_damage() const;
    int get_x() const;
    int get_y() const;
    int get_max_hp() const { return max_hp; }
    void set_max_hp(int v);
    void reset_hand(size_t cap);
};

