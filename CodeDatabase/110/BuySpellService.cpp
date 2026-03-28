#include "BuySpellService.hpp"

bool BuySpellService::performSpellBuy (SpellStore& store, SpellHand& hand, EntityManager& manager, const size_t index) {
    Player& player = manager.getPlayerPos().getEntityAs<Player>();
    if (player.getPoints() >= 3) {
        hand.addSpell(store.getSpell(index));
        store.removeSpell(index);
        return true;
    }
    return false;
}