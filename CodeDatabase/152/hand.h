#ifndef GAME_HAND_H_
#define GAME_HAND_H_

#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "spell.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
class Game;

class Hand {
public:
    explicit Hand(size_t max_size = 3);

    void add_random_spell();
    void use_spell(size_t index, Game& game, GameField& field, Player& player, int row, int col);
    void show_spells() const;

    size_t size() const { return spells_.size(); }
    void clear_spells() { spells_.clear(); }
    void add_spell(std::unique_ptr<Spell> spell) {
        if (spells_.size() < max_size_) {
            spells_.push_back(std::move(spell));
        }
    }

    size_t max_size() const { return max_size_; }

    friend class Game;

private:
    std::vector<std::unique_ptr<Spell>> spells_;
    size_t max_size_;
};

#endif
