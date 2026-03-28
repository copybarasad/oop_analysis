#pragma once

#include <vector>
#include <memory>

#include "hand_image.hpp"
#include "spell_factory.hpp"
#include "ISpell.hpp"

class Hand {
    std::vector<std::shared_ptr<ISpell>> cards;
    size_t capacity;
    int enhancement_level = 0;

public:
    Hand(size_t capacity);

    Hand(HandImage img);
    HandImage get_data_image();

    std::string get_spell_str();

    Hand(size_t capacity, std::shared_ptr<ISpell> initial_spell);

    bool add_spell(char spell);
    bool add_spell(std::shared_ptr<ISpell> spell);
    std::shared_ptr<ISpell> get_spell(size_t index);
};
