#pragma once
#include "ISpell.hpp"

class DirectDamageSpell : public ISpell {
public:
	DirectDamageSpell(int damage = 25, int range = 5);
	DirectDamageSpell(const DirectDamageSpell& other);
	DirectDamageSpell(DirectDamageSpell&& other) noexcept;
	DirectDamageSpell& operator=(const DirectDamageSpell& other);
	DirectDamageSpell& operator=(DirectDamageSpell&& other) noexcept;
	~DirectDamageSpell() override = default;

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
