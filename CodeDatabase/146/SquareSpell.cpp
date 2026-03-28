#include "SquareSpell.h"
#include "Hand.h"
#include "GameArea.h"

bool SquareSpell::apply(const SpellContext& context, GameArea& gamearea) {
    int upgrades = 0;
    if (context.caster_hand) {
        upgrades = context.caster_hand->consume_upgrades();
    }

    int boosted_radius = radius + upgrades;
    bool killed = false;

    std::vector<Coords> targets = gamearea.find_in_radius(
        context.caster_position,
        boosted_radius,
        context.target_types
    );

    std::cout << "Square spell with " << upgrades << " upgrades (area: "
        << (boosted_radius * 2) << "x" << (boosted_radius * 2) << ")\n";

    for (const auto& target : targets) {
        if (gamearea.apply_damage(target, damage, context.caster_position, false))
            killed = true;
    }
    return killed;
}
