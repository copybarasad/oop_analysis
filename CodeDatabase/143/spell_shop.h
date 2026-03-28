#ifndef SPELL_SHOP_H
#define SPELL_SHOP_H

#include "spell_card.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "player.h"
#include <vector>
#include <memory>

class GameController;

struct SpellOffer {
    std::unique_ptr<SpellCard> spell;
    std::string name;
    std::string description;
    int cost;
};

class SpellShop {
public:
    SpellShop(GameController* controller);

    const std::vector<SpellOffer>& getAvailableSpells() const;
    bool buySpell(Player& player, int spellIndex);

private:
    std::vector<SpellOffer> availableSpells;
    GameController* gameController;

    void initializeSpells();
};

#endif


