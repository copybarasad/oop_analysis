#include "upgradespell.hpp"
#include "hero.hpp"

bool UpgradeSpell::cast(Field&, EntityManager&, Hero& player, int, int) {
    player.upgrade_level++;
    return true;
}

std::string UpgradeSpell::name() const {
    return "Upgrade";
}

int UpgradeSpell::cost() const {
    return 30;
}