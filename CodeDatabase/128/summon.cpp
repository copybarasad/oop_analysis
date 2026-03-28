#include "summon.hpp"
#include "gamemodel.hpp"
#include "field.hpp"
#include "ally.hpp"
#include "constants.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"
#include "entity_snapshot_factory.hpp"


bool Summon::UseSpell(Game& game, const SpellParams& params)
{
    auto& field  = game.getField();
    auto& player = game.getPlayer();
    EntitySnapshotFactory factory;
    static const int dx4[configs::kNumberSides] = {1, -1, 0, 0};
    static const int dy4[configs::kNumberSides] = {0, 0, 1, -1};
    int px = player.getX();
    int py = player.getY();
    int summonCount = params.getSummonCount();
    int summoned = 0;
    EventDispatcher::instance().notify(UiMessageEvent("🧬 SUMMON SPELL 🧬"));
    auto casterSnap = factory.MakePlayerSnapshot(game);
    EventDispatcher::instance().notify(SpellUsedEvent(casterSnap, -1, px, py, SpellType::kSummon));
    for (int i = 0; i < configs::kNumberSides && summoned < summonCount; ++i)
    {
        int nx = px + dx4[i];
        int ny = py + dy4[i];
        if (!game.InBounds(nx, ny))
            continue;
        auto& cell = field.At(nx, ny);
        if (cell.getType() == CellType::kEmpty)
        {
            cell.set(CellType::kAlly, -1);
            game.AddAlly(nx, ny);
            ++summoned;
            EventDispatcher::instance().notify(UiMessageEvent("✨ Ally summoned at (" +std::to_string(nx) + "," + std::to_string(ny) + ")"));
        }
    }
    if (summoned == 0)
    {
        EventDispatcher::instance().notify(UiMessageEvent("❌ No space to summon an ally near the player."));
        return false;
    }
    EventDispatcher::instance().notify(UiMessageEvent("🤝 Total allies summoned: " + std::to_string(summoned)));
    return true;
}

std::string Summon::getName() const noexcept {
    return "kSummon";
}