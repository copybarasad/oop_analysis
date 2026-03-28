//
// Created by Mac on 17.11.2025.
//

#ifndef GAME_TERM_SPELLOFFERS_H
#define GAME_TERM_SPELLOFFERS_H
#include "spells/ISpell.h"

struct SpellOffer {
    std::string name;
    int cost;
    std::function<std::unique_ptr<ISpell>()> createSpell;
};

#endif //GAME_TERM_SPELLOFFERS_H