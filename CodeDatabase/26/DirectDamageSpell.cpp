#include "DirectDamageSpell.hpp"
#include "GameWorld.hpp"
#include "Player.hpp"
#include "Field.hpp"

DirectDamageSpell::DirectDamageSpell(int baseDamage,
                                     int baseRange,
                                     bool useStacks,
                                     std::optional<Position> originOverride)
	: baseDamage_(baseDamage),
	  baseRange_(baseRange),
	  useStacks_(useStacks),
	  hasOriginOverride_(originOverride.has_value()),
	  originOverride_(originOverride.value_or(Position{0, 0}))
{}

std::string DirectDamageSpell::name() const {
	return "Direct damage";
}

int DirectDamageSpell::hotkey() const noexcept {
	return 1;
}

bool DirectDamageSpell::requiresTarget() const noexcept {
	return true;
}

bool DirectDamageSpell::cast(SpellContext& ctx) {
	if (!ctx.hasTarget) {
		return false;
	}

	int effectiveRange = baseRange_;
	if (useStacks_) {
		effectiveRange += ctx.world.powerState().pendingStacks();
	}

	bool casted = false;
	if (hasOriginOverride_) {
		casted = ctx.world.dealDirectDamageFromOrigin(
			originOverride_, ctx.targetCell, baseDamage_, effectiveRange
		);
	} else {
		casted = ctx.world.dealDirectDamageAt(
			ctx.targetCell, baseDamage_, effectiveRange
		);
	}

	if (casted && useStacks_) {
		ctx.world.powerState().consumeStacksForNextSpell();
	}
	return casted;
}
