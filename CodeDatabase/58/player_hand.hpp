#pragma once

#include "waterball.hpp"
#include "poison_cloud.hpp"
#include "banana_trap.hpp"
#include <vector>
#include <random>

#define BASE_SIZE_HAND 5

class PlayerHand{
private:
    int size_hand{BASE_SIZE_HAND};
    std::vector<ISpellCard*> cards;

    int random(int max);
public:
    PlayerHand(int size_hand = BASE_SIZE_HAND);
    virtual ~PlayerHand();

    PlayerHand(const PlayerHand& other);

    int get_max_size_hand();
    int get_cur_size_hand();

    std::vector<ISpellCard*> get_cards();
    void add_random_card();
    void set_cards(std::vector<ISpellCard*>& cards);
    void set_size_hand(int size_hand);

    void remove_card(int index);
    void random_remove_card();

    int use_card(World* world);
};