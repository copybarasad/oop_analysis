#include "spell_card.hpp"
#include "game_state.hpp"
#include "map.hpp"
#include "game_object.hpp"
#include <iostream>

SpellCard::SpellCard(const std::string& spellName, const std::string& spellDesc, 
                     int spellRange)
    : name(spellName), description(spellDesc), range(spellRange) {
}

std::string SpellCard::getName() const {
    return name;
}

std::string SpellCard::getDescription() const {
    return description;
}

int SpellCard::getRange() const {
    return range;
}