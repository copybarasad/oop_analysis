#pragma once
#include "Spell.h"

class SummonSpell : public Spell {
public:
	SummonSpell(int allyHp = 8, int allyDmg = 2, int charges = 1)
		: Spell("Summon", SpellType::Summon, charges), allyHp_(allyHp), allyDmg_(allyDmg) {}

	bool canCast(const Manager& manager, EntityId casterId, const Position& target,
				 const UpgradeState* up = nullptr) const override;
	bool cast(Manager& manager, EntityId casterId, const Position& target,
			  const UpgradeState* up = nullptr) override;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
	int allyHp_{};
	int allyDmg_{};
	int extraCount_{0};
};
