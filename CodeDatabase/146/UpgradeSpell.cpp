#include "UpgradeSpell.h"
#include "Hand.h"
#include <iostream>

bool UpgradeSpell::apply(const SpellContext& context, GameArea& gamearea)
{
    if (context.caster_hand) {
        context.caster_hand->add_upgrade();
        std::cout << "Upgrade spell activated! Total upgrades: "
            << context.caster_hand->get_upgrades() << "\n";
    }
    return false;
}