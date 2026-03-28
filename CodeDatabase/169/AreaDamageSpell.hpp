#pragma once
#include "ISpell.hpp"

class AreaDamageSpell : public ISpell {
public:
	AreaDamageSpell(int damage = 15, int range = 4);
	AreaDamageSpell(const AreaDamageSpell& other);
	AreaDamageSpell(AreaDamageSpell&& other) noexcept;
	AreaDamageSpell& operator=(const AreaDamageSpell& other);
	AreaDamageSpell& operator=(AreaDamageSpell&& other) noexcept;
	~AreaDamageSpell() override = default;

	bool cast(SpellManager& spellManager, const Coords& casterPos, const Coords& target) override;
	int getRange() const noexcept override;
	int getDamage() const noexcept override;
	std::string getName() const override;
	std::string getDescription() const override;
	std::unique_ptr<ISpell> clone() const override;

private:
	int damage;
	int range;
};
