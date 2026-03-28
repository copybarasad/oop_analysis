#ifndef HAND_H
#define HAND_H

#include "spell_card.h"
#include "constants.h"
#include <vector>
#include <memory>

class Hand{
private:
    std::vector<std::unique_ptr<Spell_card>> spells;
    size_t max_size;

public:
    Hand(size_t max_size = SPELL_HAND_SIZE);
    bool add_spell(std::unique_ptr<Spell_card> spell);
    bool remove_spell(size_t index);
    void clear_hand();
    size_t get_spell_count() const;
    size_t get_max_size() const;
    const Spell_card* get_spell(size_t index) const;
    Spell_card* get_spell(size_t index);
    bool cast_spell(size_t index, Player& player, int target_x, int target_y, Game_field& field);
    void display_hand() const;
};

#endif