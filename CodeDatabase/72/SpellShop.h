//
// Created by Mac on 25.10.2025.
//

#ifndef GAME_TERM_SPELLSHOP_H
#define GAME_TERM_SPELLSHOP_H
#include <functional>
#include <string>
#include "field_objects/Player.h"
#include "spells/ISpell.h"
#include "SpellOffers.h"

class SpellShop {
    std::vector<SpellOffer> offers;

    std::vector<SpellOffer> createOffers(const int & currentLevel);

public:
    explicit SpellShop(const int & currentLevel);
    void showOffers() const;
    void buySpell(Player& player);
};


#endif //GAME_TERM_SPELLSHOP_H