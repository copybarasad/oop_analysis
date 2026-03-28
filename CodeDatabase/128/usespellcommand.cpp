#include "usespellcommand.hpp"
#include "gamemodel.hpp"
#include "player.hpp"
#include "hand.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"
#include <iostream>

bool UseSpellCommand::Execute(Game& game)
{
    Player& player = game.getPlayer();
    Hand& hand = player.getHand();
    auto& spells = hand.getSpells();
    if (spells.empty()) {
        EventDispatcher::instance().notify(UiMessageEvent("⚠️ You have no spells.\n"));
        return false;
    }
    EventDispatcher::instance().notify(UiMessageEvent("\n=== Your Spells ===\n"));
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << (i + 1) << " - " << spells[i]->getName() << "\n";
    }
    
    EventDispatcher::instance().notify(UiMessageEvent("Choose spell index: "));
    size_t index;
    std::cin >> index;
    return hand.UseSpell(index, game);
}
