#include "include/BuffSpell.h"
#include "../gamecore/include/Manager.h"

bool BuffSpell::canCast(const Manager& manager, EntityId casterId, const Position&, const UpgradeState* up) const {
	(void)manager; (void)casterId; (void)up;
	return hasCharges();
}

bool BuffSpell::cast(Manager& manager, EntityId casterId, const Position& target, const UpgradeState* up) {
	(void)up;
	if (!canCast(manager, casterId, target, up)) return false;
	return consumeOne();
}

void BuffSpell::save(std::ostream& os) const {
    Spell::save(os);
}

void BuffSpell::load(std::istream& is) {
    Spell::load(is);
}
