#pragma once
#include "Spell.h"


class TrapSpell : public Spell 
{
public:
	TrapSpell(std::string name, int damage);

	std::string GetName() const override ;
	void ApplyBuffs(BuffState const& state) override;
	bool Apply(Player& caster, Game& game, Position target) override;

private:
	std::string name_;
	int damage_;
};