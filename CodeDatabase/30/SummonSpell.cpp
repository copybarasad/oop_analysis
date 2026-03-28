#include "include/SummonSpell.h"
#include "../gamecore/include/Manager.h"
#include "../entities/include/Ally.h"
#include <iostream>

bool SummonSpell::canCast(const Manager& manager, EntityId casterId, const Position& target,
						  const UpgradeState* up) const {
	(void)target; (void)up; // тут важно только, что есть свободная клетка рядом
    auto cposOpt = getCasterPosIfValid(manager, casterId);
    if (!cposOpt) return false;
    Position c = *cposOpt;

	bool anyFree = false;
	for (int dr = -1; dr <= 1; ++dr) {
		for (int dc = -1; dc <= 1; ++dc) {
			if (std::abs(dr) + std::abs(dc) != 1) continue; // только манхэттен соседи
			Position np{c.row + dr, c.col + dc};
			if (!manager.inBounds(np)) continue;
			if (!manager.isPassable(np)) continue;
			if (manager.entities().occupantAt(np).has_value()) continue;
			anyFree = true; break;
		}
		if (anyFree) break;
	}
	return anyFree; // наличие хотя бы одной свободной клетки достаточно
}

bool SummonSpell::cast(Manager& manager, EntityId casterId, const Position& target,
                       const UpgradeState* up) {
    if (!canCast(manager, casterId, target, up)) return false;
	if (!consumeOne()) return false;
	auto cposOpt = manager.entities().positionOf(casterId);
	if (!cposOpt) return false;
	Position c = *cposOpt;
	int bonus = up ? up->stacks : 0;
	int toSpawn = 1 + bonus;
	int spawned = 0;
	for (int dr = -1; dr <= 1 && spawned < toSpawn; ++dr) {
		for (int dc = -1; dc <= 1 && spawned < toSpawn; ++dc) {
			if (std::abs(dr) + std::abs(dc) != 1) continue;
			Position np{c.row + dr, c.col + dc};
			if (!manager.inBounds(np)) continue;
			if (!manager.isPassable(np)) continue;
			if (manager.entities().occupantAt(np).has_value()) continue;
			auto ally = std::make_unique<Ally>(allyHp_, allyDmg_);
			if (manager.entities().spawnAt(std::move(ally), np) != InvalidEntityId) {
				++spawned;
			}
		}
	}
	return spawned > 0; // частичный успех допустим
}

void SummonSpell::save(std::ostream& os) const {
    Spell::save(os);
    os << allyHp_ << " " << allyDmg_ << " " << extraCount_ << "\n";
}

void SummonSpell::load(std::istream& is) {
    Spell::load(is);
    is >> allyHp_ >> allyDmg_ >> extraCount_;
}
