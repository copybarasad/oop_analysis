#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include <vector>
#include <memory>
#include "spell.h"

class Game_controller;
class Position;

class SpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int max_size;
    int enemies_defeated;
    int enemies_for_new_spell;

public:
    SpellHand(int max_spells = 3, int enemies_per_spell = 3);
    
    bool add_spell(std::unique_ptr<Spell> spell);
    bool remove_spell(int index);
    void cast_spell(int index, Game_controller& controller, const Position& target);
    void on_enemy_defeated();
    bool can_get_new_spell() const;
    void get_random_spell();
    
    void clear_spells();
    void set_enemies_defeated(int count) { enemies_defeated = count; }
    void set_enemies_for_new_spell(int count) { enemies_for_new_spell = count; }
    
    int get_enemies_defeated() const { return enemies_defeated; }
    int get_enemies_for_new_spell() const { return enemies_for_new_spell; }

    const std::vector<std::unique_ptr<Spell>>& get_spells() const { return spells; }
    std::vector<std::unique_ptr<Spell>>& get_spells() { return spells; }
    int get_current_size() const { return spells.size(); }
    int get_max_size() const { return max_size; }
    bool is_full() const { return spells.size() >= max_size; }
    
    void display_spells() const;
};

#endif