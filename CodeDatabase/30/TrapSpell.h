#pragma once
#include "Spell.h"

class TrapSpell : public Spell {
public:
	TrapSpell(int trapDamage = 6, int charges = 1)
		: Spell("Trap", SpellType::Trap, charges), trapDamage_(trapDamage) {}

	bool canCast(const Manager& manager, EntityId casterId, const Position& target,
				 const UpgradeState* up = nullptr) const override;
	bool cast(Manager& manager, EntityId casterId, const Position& target,
			  const UpgradeState* up = nullptr) override;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
	int trapDamage_{};
};
