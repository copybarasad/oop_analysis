#include "include/AreaDamageSpell.h"
#include "../gamecore/include/Manager.h"
#include "../entities/include/Entity.h"
#include "../gamecore/include/MathUtils.h"
#include <algorithm>

bool AreaDamageSpell::canCast(const Manager& manager, EntityId casterId, const Position& target,
								  const UpgradeState* /*up*/) const {
    auto cposOpt = getCasterPosIfValid(manager, casterId);
    if (!cposOpt) return false;
    Position cpos = *cposOpt;

	if (!manager.inBounds(target)) return false;
	if (manhattan(cpos, target) > radius_) return false;
	return true;
}

bool AreaDamageSpell::cast(Manager& manager, EntityId casterId, const Position& target,
                           const UpgradeState* up) {
    if (!canCast(manager, casterId, target, up)) return false;
	if (!consumeOne()) return false;
	int bonus = up ? up->stacks : 0;
	int side = std::max(2, 2 + bonus);
    int offset = side / 2; 

	for (int dr = 0; dr < side; ++dr) {
		for (int dc = 0; dc < side; ++dc) {
			Position p{target.row + dr - offset, target.col + dc - offset};
			if (!manager.inBounds(p)) continue;

			auto idOpt = manager.occupantAt(p);
			if (!idOpt) continue;

			Entity* e = manager.entities().entity(*idOpt);
			if (!e) continue;

			auto t = e->type();
			if (t == EntityType::Enemy || t == EntityType::EnemyBuilding || t == EntityType::EnemyTower) {
				manager.combat().damageAndResolve(casterId, *idOpt, baseDamage_);
			}
		}
	}
	return true;
}

void AreaDamageSpell::save(std::ostream& os) const {
    Spell::save(os);
    os << baseDamage_ << " " << radius_ << "\n";
}

void AreaDamageSpell::load(std::istream& is) {
    Spell::load(is);
    is >> baseDamage_ >> radius_;
}
