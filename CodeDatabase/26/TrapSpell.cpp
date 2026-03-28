#include "TrapSpell.hpp"
#include "GameWorld.hpp"
#include "Player.hpp"
#include "Field.hpp"

TrapSpell::TrapSpell(int trapDamage, int placementRange, int trapLifetime)
	: trapDamage_(trapDamage),
	  placementRange_(placementRange),
	  trapLifetime_(trapLifetime)
{}

std::string TrapSpell::name() const {
	return "Trap";
}

int TrapSpell::hotkey() const noexcept {
	return 3;
}

bool TrapSpell::requiresTarget() const noexcept {
	return true;
}

bool TrapSpell::cast(SpellContext& ctx) {
	if (!ctx.hasTarget) {
		return false;
	}

	const int bonusDamage = ctx.world.powerState().pendingStacks();
	const int effectiveDamage = trapDamage_ + bonusDamage;
	const bool placed = ctx.world.placeTrapAt(ctx.targetCell,
	                                         effectiveDamage,
	                                         placementRange_,
	                                         trapLifetime_);
	if (placed) {
		ctx.world.powerState().consumeStacksForNextSpell();
	}
	return placed;
}
