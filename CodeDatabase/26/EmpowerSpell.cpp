#include "EmpowerSpell.hpp"
#include "GameWorld.hpp"
#include "Player.hpp"

EmpowerSpell::EmpowerSpell(int stackValue)
	: stackValue_(stackValue)
{}

std::string EmpowerSpell::name() const {
	return "Empower";
}

int EmpowerSpell::hotkey() const noexcept {
	return 5;
}

bool EmpowerSpell::requiresTarget() const noexcept {
	return false;
}

bool EmpowerSpell::cast(SpellContext& ctx) {
	ctx.world.powerState().addStacks(stackValue_);
	return true;
}
