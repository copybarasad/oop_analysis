#include "trapped.hpp"
#include "gamemodel.hpp"
#include "cell.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"
#include "entity_snapshot_factory.hpp"
#include "enums.hpp"

#include <iostream>

bool Trapped::UseSpell(Game& game, const SpellParams& params)
{
    auto& field = game.getField();
    EntitySnapshotFactory factory;
    EventDispatcher::instance().notify(UiMessageEvent("🎯 Enter coordinates to place trap (x y):"));
    int tx, ty;
    std::cin >> tx >> ty;
    if (!game.InBounds(tx, ty)) {
        EventDispatcher::instance().notify(UiMessageEvent("❌ Invalid coordinates! Target outside the field."));
        return false;
    }
    auto& cell = field.At(tx, ty);
    if (cell.getType() != CellType::kEmpty) {
        EventDispatcher::instance().notify(UiMessageEvent("⚠️ Can't place trap here — cell is not empty."));
        return false;
    }
    auto casterSnap = factory.MakePlayerSnapshot(game);
    EventDispatcher::instance().notify(SpellUsedEvent(casterSnap, -1, tx, ty, SpellType::kTrapped));
    game.AddTrap(tx, ty, params.getTrapDamage());
    EventDispatcher::instance().notify(UiMessageEvent("💣 Trap placed at (" + std::to_string(tx) + "," + std::to_string(ty) + ")!")
    );
    return true;
}

std::string Trapped::getName() const noexcept {
    return "kTrapped";
}
