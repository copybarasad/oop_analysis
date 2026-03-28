#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include "spell_hand.h"

class Game_controller;

enum class Combat_style {
    CLOSE,
    RANGED
};

class Player : public Character {
private:
    int score;
    Combat_style combat_style;
    int close_damage;
    int ranged_damage;
    SpellHand spell_hand;

public:
    Player(int health, int close_damage_value, int ranged_damage_value, 
           Position start_position, int max_spells = 3);
    
    void switch_combat_style(Combat_style new_style);
    Combat_style get_combat_style() const;
    int get_score() const;
    void add_score(int points);
    void take_damage(int damage_amount) override;
    
    void cast_spell(int spell_index, Game_controller& controller, const Position& target);
    void on_enemy_defeated();
    void try_get_new_spell();
    void display_spells() const;
    
    void level_up();
    void upgrade_spells(int damage_bonus);
    void upgrade_close_damage(int bonus);
    void upgrade_ranged_damage(int bonus);
    
    SpellHand& get_spell_hand() { return spell_hand; }
    const SpellHand& get_spell_hand() const { return spell_hand; }
    
    int get_close_damage() const { return close_damage; }
    int get_ranged_damage() const { return ranged_damage; }
};

#endif