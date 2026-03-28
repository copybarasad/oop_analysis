#include "SpellPowerState.hpp"

SpellPowerState::SpellPowerState() noexcept
	: pendingStacks_(0) {}

void SpellPowerState::addStacks(int stacks) {
	if (stacks > 0) {
		pendingStacks_ += stacks;
	}
}

int SpellPowerState::consumeStacksForNextSpell() {
	int applied = pendingStacks_;
	pendingStacks_ = 0;
	return applied;
}

int SpellPowerState::pendingStacks() const noexcept {
	return pendingStacks_;
}

void SpellPowerState::setStacks(int stacks) {
	if (stacks < 0) stacks = 0;
	pendingStacks_ = stacks;
}

SpellPowerState::~SpellPowerState() {}
