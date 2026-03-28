#pragma once
#include <memory>
#include <string>
#include <vector>

#include "ispell.h"
#include "spellid.h"

class Grid;
class Player;

class Hand {
public:
    explicit Hand(size_t capacity);

    size_t size() const { return spells.size(); }
    size_t max_size() const { return capacity; }
    const std::vector<std::unique_ptr<ISpell>>& get_spells() const { return spells; }

    bool add_spell(std::unique_ptr<ISpell> spell);
    bool add_random_spell();
    void remove_random_half();

    std::vector<std::string> ListSpells() const;

    bool buy_spell(int score);
    bool cast_spell_by_index(size_t index, Player& player, Grid& grid, int tx, int ty);

    void clear_spells() { spells.clear(); }

    static std::unique_ptr<ISpell> CreateSpellForId(SpellId id);

private:
    static std::unique_ptr<ISpell> make(SpellId id);
    bool has_spell(SpellId id) const;

    std::vector<std::unique_ptr<ISpell>> spells;
    size_t capacity;
};
