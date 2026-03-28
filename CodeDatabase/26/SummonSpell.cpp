#include "SummonSpell.hpp"
#include "GameWorld.hpp"
#include "Player.hpp"
#include "Field.hpp"
#include "SpellLibrary.hpp"

SummonSpell::SummonSpell(int allyHp, int allyDamage, int allyCount)
	: allyHp_(allyHp),
	  allyDamage_(allyDamage),
	  allyCount_(allyCount)
{}

std::string SummonSpell::name() const {
	return "Summon ally";
}

int SummonSpell::hotkey() const noexcept {
	return 4;
}

bool SummonSpell::requiresTarget() const noexcept {
	return false;
}

bool SummonSpell::cast(SpellContext& ctx) {
	const auto stats = SpellLibrary::summonStats();
	const int bonusAllies = ctx.world.powerState().pendingStacks();
	const int totalAllies = stats.count + bonusAllies;
	if (totalAllies <= 0) {
		return false;
	}
	const bool summoned = ctx.world.summonAlliesNearPlayer(totalAllies, stats.hp, stats.damage);
	if (summoned) {
		ctx.world.powerState().consumeStacksForNextSpell();
	}
	return summoned;
}

