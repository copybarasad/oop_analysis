#pragma once
#include <memory>
#include <vector>
#include <utility>
#include "Spell.h"
#include "BuffSpell.h"
#include "../../gamecore/include/UpgradeState.h"

class Hand {
public:
	explicit Hand(int maxSlots = 5);

	int slotsCount() const { return (int)slots_.size(); }
	int maxSlots() const { return maxSlots_; }
	bool hasFreeSlot() const { return slots_.size() < (size_t)maxSlots_; }

	int size() const { return slotsCount(); }
	int capacity() const { return maxSlots(); }

	void addSpell(std::unique_ptr<Spell> s);
	void addRandomSpell();
	void select(size_t index);
	Spell* current();

	// Баффы тут копят стаки и работают только на следующий каст.
	UpgradeState& upgrades();
	void resetUpgrades();
	bool useCurrent(Manager& mng, EntityId caster, const Position& target);

	void removeRandomSpells(double ratio);

    void save(std::ostream& os) const;
    void load(std::istream& is);

	const std::vector<std::unique_ptr<Spell>>& slots() const { return slots_; }

private:
	std::vector<std::unique_ptr<Spell>> slots_;
	size_t current_{0};
	int maxSlots_{};
	UpgradeState upgrades_{};
};
