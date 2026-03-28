#pragma once
#include "Spell.h"

class DirectDamageSpell : public Spell {
public:
	DirectDamageSpell(int baseDamage = 8, int range = 3, int charges = 1)
		: Spell("DirectDamage", SpellType::DirectDamage, charges), baseDamage_(baseDamage), range_(range) {}

	bool canCast(const Manager& manager, EntityId casterId, const Position& target,
				 const UpgradeState* up = nullptr) const override;
	bool cast(Manager& manager, EntityId casterId, const Position& target,
			  const UpgradeState* up = nullptr) override;

	int range() const { return range_; }
	int baseDamage() const { return baseDamage_; }
	void increaseRange(int d) { range_ += d; }

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
	int baseDamage_{};
	int range_{};
};