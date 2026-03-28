#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "AllSpells.h"
#include <vector>
#include <memory>

class ISpell;
class GameField;
class Player;

class SpellHand{
public:
    SpellHand(int max_size = 3)
        : max_size_(max_size) { fill_random_spells(1); }

    ~SpellHand();
    
    bool add_spell(ISpell* spell);
    bool remove_spell(int index);

    bool cast_spell(GameField& game_field, Player& player, int target_x = -1, int target_y = -1, int index = 0);

    int get_size() const {return spells_.size(); }
    int get_max_size() const { return max_size_; }
    bool is_full() const { return spells_.size() >= max_size_; }
    ISpell* get_spell(int index) const;
    void reset_all_spells();

    void display_spells() const;
    void fill_random_spells(int count);

private:
    std::vector<ISpell*> spells_;
    int max_size_;
};

#endif