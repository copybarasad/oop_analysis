#ifndef SPELL_STORE_S_H
#define SPELL_STORE_S_H

#include "../../Spells/SpellHand/SpellHand.hpp"
#include "../../Managers/EntityManager/EntityManager.hpp"
#include "../../Spells/SpellStore/SpellStore.hpp"
#include "../../Entities/Player/Player.hpp"

class BuySpellService {
    public:
    BuySpellService() = default;

    bool performSpellBuy (SpellStore& store, SpellHand& hand, EntityManager& manager, const size_t index);
};

#endif