#include "AreaDamageSpell.hpp"
#include "GameWorld.hpp"
#include "Player.hpp"
#include "Field.hpp"

AreaDamageSpell::AreaDamageSpell(int baseDamage, int areaSize, int baseRange)
	: baseDamage_(baseDamage),
	  areaSize_(areaSize),
	  baseRange_(baseRange)
{
}

std::string AreaDamageSpell::name() const {
	return "Area damage";
}

int AreaDamageSpell::hotkey() const noexcept {
	return 2;
}

bool AreaDamageSpell::requiresTarget() const noexcept {
	return true;
}

bool AreaDamageSpell::cast(SpellContext& ctx) {
	Position target = ctx.targetCell;
	if (!ctx.hasTarget) {
		auto playerPos = ctx.world.playerPosition();
		if (!playerPos.has_value()) {
			return false;
		}
		target = *playerPos;
	}

	const int bonusArea = ctx.world.powerState().pendingStacks();
	const int effectiveArea = areaSize_ + bonusArea;
	const bool resolved = ctx.world.dealAreaDamageAt(target, effectiveArea, baseDamage_, baseRange_);
	if (resolved) {
		ctx.world.powerState().consumeStacksForNextSpell();
	}
	return resolved;
}
