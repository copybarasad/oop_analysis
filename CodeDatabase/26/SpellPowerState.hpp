#pragma once
#include <cstddef>

class SpellPowerState {
public:
	SpellPowerState() noexcept;

	void addStacks(int stacks);
	int consumeStacksForNextSpell();
	int pendingStacks() const noexcept;
	void setStacks(int stacks);

	~SpellPowerState();

private:
	int pendingStacks_;
};
