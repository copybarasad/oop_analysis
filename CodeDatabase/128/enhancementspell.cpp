#include "enhancementspell.hpp"
#include "player.hpp"
#include "hand.hpp"
#include "constants.hpp"

#include "event_dispatcher.hpp"
#include "events.hpp"
#include "entity_snapshot_factory.hpp"

using namespace configs;

bool EnhancementSpell::UseSpell(Game& game, const SpellParams&)
{
    auto& player = game.getPlayer();
    auto& hand   = player.getHand();
    EntitySnapshotFactory factory;
    auto casterSnap = factory.MakePlayerSnapshot(game);
    EventDispatcher::instance().notify(
        SpellUsedEvent(casterSnap, -1, player.getX(), player.getY(), SpellType::kEnhancement)
    );
    hand.AddEnhancement(kEnhancementValue);
    EventDispatcher::instance().notify(
        UiMessageEvent(
            "✨ Magical energy increases! (Total stacks: "
            + std::to_string(hand.getEnhancementStacks()) + ")"
        )
    );
    return true;
}

std::string EnhancementSpell::getName() const noexcept {
    return "kEnhancement";
}
