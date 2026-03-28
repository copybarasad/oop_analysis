#pragma once
#include "vector"
#include "string"

class SpellCard;

class Hand {
private:
    std::vector<SpellCard*> hand;
    int upgrade_count = 0;
    int count_kill_for_spell = 1;
    int kill_counter = 0;
    int hand_size;
public:
    Hand(int h_s) : hand_size(h_s) {
        add_random_spell();
    }
    SpellCard* create_spell_by_index(int index);
    void add_random_spell();
    void add_upgrade() { upgrade_count++; }
    void plus_kill();
    std::vector<SpellCard*>& get_spells() { return hand; }
    int get_hand_size() const { return hand_size; }
    int get_upgrades() const { return upgrade_count; }
    int get_count_kill() { return kill_counter; }
    int consume_upgrades();
    void clear_half_hand();
    std::string print();
    void add_slot() { hand_size++; };
};