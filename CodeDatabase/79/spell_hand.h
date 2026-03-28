#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include <memory>
#include <cstdlib>
#include <vector>

#include "imagic_spell_card.h"
class Player;
class Place;
class Enemy;
class Tower;


class IMagic_spell_card;
class Tower;
class SpellHand{
private:
    std::vector<std::unique_ptr<IMagic_spell_card>> spells;
    int max_size = 3;
public:
    SpellHand();

    SpellHand(const SpellHand&) = delete;
    SpellHand& operator=(const SpellHand&) = delete;
    
    SpellHand(SpellHand&&) = default;
    SpellHand& operator=(SpellHand&&) = default;

    int add_spell(std::unique_ptr<IMagic_spell_card> spell);
    int use_spell(int num, Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
    int buy_spell(Player& player);

    void create_random_spell();

    void getInfo();
    int getSize() const;
    int getMaxSize() const;
    const IMagic_spell_card* getSpell(int num) const;
    bool is_full() const;
    int remove_spell(int index);
    int remove_random_spell();
    std::vector<int> get_spell_types() const;
    void load_spells(const std::vector<int>& spell_types);
    bool upgrade(int spell_ind, int value);
};

#endif