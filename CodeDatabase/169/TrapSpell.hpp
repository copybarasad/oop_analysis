#pragma once
#include "ISpell.hpp"

class TrapSpell : public ISpell {
public:
	TrapSpell(int damage = 30, int range = 3);
	TrapSpell(const TrapSpell& other);
	TrapSpell(TrapSpell&& other) noexcept;
	TrapSpell& operator=(const TrapSpell& other);
	TrapSpell& operator=(TrapSpell&& other) noexcept;
	~TrapSpell() override = default;

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
