#include "Tower.h"
#include "GameArea.h"
#include "DirectSpell.h"

void Tower::do_turn(GameArea& gamearea)
{
    if (step < step_cd) {
        step++;
        return;
    }

    std::vector<Coords> targets = gamearea.find_in_radius(coords, radius, { "Player" });
    if (!targets.empty()) {
        DirectSpell spell(damage, radius);
        SpellContext context(
            coords,
            { "Player", "Summon" },
            nullptr  // Передаем указатель на руку
        );
        spell.apply(context, gamearea);
        step = 0;
    }

}