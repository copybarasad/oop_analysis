#include "TrapSpell.h"
#include "Trap.h"
#include "Hand.h"
#include "GameArea.h"

bool TrapSpell::apply(const SpellContext& context, GameArea& gamearea) {
    int upgrades = 0;
    if (context.caster_hand) {
        upgrades = context.caster_hand->consume_upgrades();
    }

    double boosted_damage = damage + upgrades * 5;

    std::vector<Coords> empty_cells = gamearea.find_in_radius(
        context.caster_position,
        radius,
        { "Empty" }
    );

    if (empty_cells.empty()) {
        std::cout << "No empty cells in radius\n";
        return false;
    }

    std::cout << "Choose cell for trap (damage: " << boosted_damage << ")\n";
    for (size_t i = 0; i < empty_cells.size(); ++i) {
        std::cout << i << ": (" << empty_cells[i].x << "," << empty_cells[i].y << ") ";
    }
    std::cout << '\n';

    int index = 0;
    std::cin >> index;

    if (index >= 0 && index < empty_cells.size()) {
        Trap* trap = new Trap(empty_cells[index], boosted_damage);
        gamearea.set_trap(trap);
    }
    return false;
}
