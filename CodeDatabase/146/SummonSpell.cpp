#include "SummonSpell.h"
#include "Hand.h"
#include "GameArea.h"

bool SummonSpell::apply(const SpellContext& context, GameArea& gamearea) {
    int upgrades = 0;
    if (context.caster_hand) {
        upgrades = context.caster_hand->consume_upgrades();
    }

    int boosted_allies = allies_count + upgrades;

    std::vector<Coords> empty_cells = gamearea.find_in_radius(
        context.caster_position,
        summon_range,
        { "Empty" }
    );

    int count = std::min(boosted_allies, static_cast<int>(empty_cells.size()));

    std::cout << "Summoning " << count << " allies\n";

    for (int i = 0; i < count; ++i) {
        gamearea.add_ally(health_ally, damage_ally, empty_cells[i]);
    }
    return false;
}
