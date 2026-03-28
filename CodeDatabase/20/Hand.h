#pragma once
#include <stack>
#include <memory>
#include "Spell.h"
#include "SpellFactory.h"

class Hand{
    private:

    std::stack<std::shared_ptr<Spell>> spells;
    int capacity;

    public:
    Hand();
    void add_random_spell();
    std::shared_ptr<Spell> get_spell();
    std::stack<std::shared_ptr<Spell>>& get_all_spells() { return spells; }


};
