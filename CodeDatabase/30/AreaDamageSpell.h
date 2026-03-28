#pragma once
#include "Spell.h"

// Бьёт квадратом: (2+бафф) x (2+бафф). target — это верхний левый угол.
class AreaDamageSpell : public Spell {
public:
	AreaDamageSpell(int baseDamage = 4, int baseRadius = 1, int charges = 1)
		: Spell("AreaDamage", SpellType::AreaDamage, charges), baseDamage_(baseDamage), radius_(baseRadius) {}

	bool canCast(const Manager& manager, EntityId casterId, const Position& target,
				 const UpgradeState* up = nullptr) const override; // будет таргетируемая область
	bool cast(Manager& manager, EntityId casterId, const Position& target,
			  const UpgradeState* up = nullptr) override;
	void increaseRadius(int r) { radius_ += r; }

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
	int baseDamage_{};
	int radius_{}; // радиус для проверки достижимости области
};
