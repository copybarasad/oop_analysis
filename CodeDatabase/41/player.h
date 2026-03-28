#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "game_field.h"
#include "hand.h" 

class Game_field;

class Player{
private:
    std::string name;
    int health;
    int max_health;
    int melee_damage; 
    int ranged_damage;
    int score;
    int level;
    Combat_mode combat_mode;
    bool is_slowed;
    int mana;
    int max_mana;
    Hand spell_hand;

protected:
    Game_field* field;

public:
    Player(const std::string& player_name, Game_field* game_field);
    std::string get_name() const;
    int get_health() const;
    int get_max_health() const;
    int get_damage() const;
    int get_score() const;
    int get_level() const;
    Combat_mode get_combat_mode() const;
    bool get_is_slowed() const;
    int get_melee_damage() const;
    int get_ranged_damage() const;
    bool is_alive() const { return health > 0; }

    void set_health(int new_health);
    void set_slowed(bool slowed);
    Move_status move(int delta_x, int delta_y);
    void take_damage(int damage_amount);
    void heal();
    void increase_score(int points);
    void level_up();
    void display_status() const;
    void switch_combat_mode();
    void perform_attack(); 

    //улучшения при поднятии уровня
    void upgrade_max_health(int amount);
    void upgrade_melee_damage(int amount);
    void upgrade_ranged_damage(int amount);
    void upgrade_max_mana(int amount);
    void enhance_random_spell();


    void set_field(Game_field* game_field){field = game_field;};
    //заклинания
    Hand& get_spell_hand();
    const Hand& get_spell_hand() const; 
    bool cast_spell(int spell_index, int target_x, int target_y);
    void add_random_spell();
    bool buy_spell(int cost);
    bool learn_spell(std::unique_ptr<Spell_card> spell);
    void learn_random_spell();
    // Мана
    int get_mana() const;
    int get_max_mana() const;
    void restore_mana(int amount);
    void set_mana(int new_mana);
    bool spend_mana(int amount);

    void set_max_health(int value) { max_health = value; }
    void set_melee_damage(int value) { melee_damage = value; }
    void set_ranged_damage(int value) { ranged_damage = value; }
    void set_score(int value) { score = value; }
    void set_level(int value) { level = value; }
    void set_combat_mode(Combat_mode mode) { combat_mode = mode; }
    void set_max_mana(int value) { max_mana = value; }
};

#endif