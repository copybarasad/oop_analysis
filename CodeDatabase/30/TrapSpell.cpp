#include "include/TrapSpell.h"
#include "../gamecore/include/Manager.h"
#include "../gamecore/include/Field.h"

bool TrapSpell::canCast(const Manager& manager, EntityId casterId, const Position& target,
                        const UpgradeState* up) const {
    (void)up;
    auto cposOpt = getCasterPosIfValid(manager, casterId);
    if (!cposOpt) return false;
    Position c = *cposOpt;

	if (!manager.inBounds(target)) return false;
	if (!manager.isPassable(target)) return false;
	if (manager.entities().occupantAt(target).has_value()) return false;
	
	int d = std::abs(c.row - target.row) + std::abs(c.col - target.col);
	if (d != 1) return false;
	return true;
}

bool TrapSpell::cast(Manager& manager, EntityId casterId, const Position& target,
                     const UpgradeState* up) {
    if (!canCast(manager, casterId, target, up)) return false;
	if (!consumeOne()) return false;
	int effectiveTrap = trapDamage_ + (up ? up->stacks : 0);
	manager.field().cellAt(target).placeTrap(effectiveTrap);
	return true;
}

void TrapSpell::save(std::ostream& os) const {
    Spell::save(os);
    os << trapDamage_ << "\n";
}

void TrapSpell::load(std::istream& is) {
    Spell::load(is);
    is >> trapDamage_;
}