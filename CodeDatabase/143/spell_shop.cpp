#include "spell_shop.h"
#include "game_controller.h"

SpellShop::SpellShop(GameController* controller)
    : gameController(controller) {
    initializeSpells();
}

void SpellShop::initializeSpells() {
    availableSpells.clear();

    availableSpells.push_back({
        std::make_unique<DirectDamageSpell>(
            "Fire Bolt",
            "Deals 25 damage to single target within 3 cells",
            25, 3, gameController
        ),
        "Fire Bolt",
        "Deals 25 damage to single target within 3 cells",
        100
        });

    availableSpells.push_back({
        std::make_unique<AreaDamageSpell>(
            "Fireball",
            "Deals 20 damage in 2x2 area within 4 cells",
            20, 4, 2, gameController
        ),
        "Fireball",
        "Deals 20 damage in 2x2 area within 4 cells",
        200
        });
}

const std::vector<SpellOffer>& SpellShop::getAvailableSpells() const {
    return availableSpells;
}

bool SpellShop::buySpell(Player& player, int spellIndex) {
    if (spellIndex < 0 || spellIndex >= availableSpells.size()) {
        return false;
    }

    const auto& offer = availableSpells[spellIndex];

    std::unique_ptr<SpellCard> spell;
    if (offer.name == "Fire Bolt") {
        spell = std::make_unique<DirectDamageSpell>(
            "Fire Bolt",
            "Deals 25 damage to single target within 3 cells",
            25, 3, gameController
        );
    }
    else if (offer.name == "Fireball") {
        spell = std::make_unique<AreaDamageSpell>(
            "Fireball",
            "Deals 20 damage in 2x2 area within 4 cells",
            20, 4, 2, gameController
        );
    }

    if (spell && player.buySpell(std::move(spell), offer.cost)) {
        return true;
    }

    return false;
}