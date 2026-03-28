#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include "spell_interface.h"
#include "area_damage_spell_interface.h"
#include "direct_damage_spell_interface.h"
#include "trap_spell_interface.h"

class SpellHand
{
private:
    std::vector<std::unique_ptr<ISpell>> spells;
    size_t max_size;

    std::unique_ptr<ISpell> create_random_spell() const;

public:
    static constexpr size_t DEFAULT_MAX_SIZE = 4;

    SpellHand();
    SpellHand(const SpellHand& other);
    SpellHand(SpellHand&& other) noexcept;
    SpellHand& operator=(const SpellHand& other);
    SpellHand& operator=(SpellHand&& other) noexcept;
    
    ISpell* get_spell(size_t index) const;
    size_t get_spell_count() const;
    size_t get_max_size() const;
    IDirectDamageSpell* get_direct_damage_spell(size_t index) const;
    IAreaDamageSpell* get_area_damage_spell(size_t index) const;
    ITrapSpell* get_trap_spell(size_t index) const;

    void set_max_size(size_t new_max_size);

    bool is_full() const;
    bool is_empty() const;

    bool add_spell(std::unique_ptr<ISpell> spell);
    void clear_hand();
    void remove_half_random();
    void increase_max_size();

    void update_cooldowns();
    
    void initialize_hand();

    void save_binary(std::ostream& os) const;
    void load_binary(std::istream& is);
};

#endif