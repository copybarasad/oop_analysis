#include "spell_card.h"
#include "player.h"
#include "game_field.h"
#include <cmath>

Spell_card::Spell_card(const std::string& spell_name, const std::string& desc, int cost, int spell_range)
    : name(spell_name), description(desc), mana_cost(cost), range(spell_range), enhanced(false) {
}

std::string Spell_card::get_name() const{ 
    return name; 
}

std::string Spell_card::get_description() const{ 
    return description; 
}

int Spell_card::get_mana_cost() const{ 
    return mana_cost; 
}

int Spell_card::get_range() const{ 
    return range; 
}