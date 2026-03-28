#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include <vector>
#include <memory>
#include <random>
#include "spell.h"

class SpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells_;
    int max_size_;

public:
    explicit SpellHand(int max_size);
    
    bool add_spell(std::unique_ptr<Spell> spell);
    bool remove_spell(int index);
    void clear_hand();
    
    Spell* get_spell(int index) const;
    int get_spell_count() const;
    int get_max_size() const;
    bool is_full() const;
    
    void print_spells() const;
    std::unique_ptr<Spell> get_random_spell() const;
    
    void remove_random_spells(int count);
};

#endif