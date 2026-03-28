#include "DirectSpell.h"
#include "Hand.h"
#include "GameArea.h"

bool DirectSpell::apply(const SpellContext& context, GameArea& gamearea) {
    int upgrades = 0;
    if (context.caster_hand) {
        upgrades = context.caster_hand->consume_upgrades();
    }

    int boosted_radius = radius + upgrades;

    std::vector<Coords> targets = gamearea.find_in_radius(
        context.caster_position,
        boosted_radius,
        context.target_types
    );

    if (!targets.empty()) {
        std::cout << "Direct spell with " << upgrades << " upgrades (radius: " << boosted_radius << ")\n";
        return gamearea.apply_damage(targets[0], damage, context.caster_position, false);
    }
    return false;
}
