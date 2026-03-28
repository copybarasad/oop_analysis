#ifndef OOP_SPELL_HAND_H
#define OOP_SPELL_HAND_H

#include <vector>
#include "i_spell.h"
#include "position.h"


class SpellHand {
    std::vector<ISpell*> spells;
    size_t max_size;

public:
    explicit SpellHand(size_t max_size = 5);
    ~SpellHand();

    void add_spell(ISpell* spell);

    void cast_spell(int index, Player& caster, GameState& game_state);


    // NEW
    bool can_cast_spell(int index, const Position& player_pos, GameState& game_state) const;
    void load_spells_from_names(const std::vector<std::string>& spell_names);


    size_t get_spell_count() const;
    std::string get_spell_name(size_t index) const;

    void clear_spells();

};

#endif //OOP_SPELL_HAND_H