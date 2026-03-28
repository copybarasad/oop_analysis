#pragma once
#include "Spell.h"
#include <vector>

class SummonSpell : public Spell 
{
public:
	SummonSpell(std::string name, int count, int range);

	std::string GetName() const override ;
	void ApplyBuffs(BuffState const& state) override;
	bool Apply(Player& caster, Game& game, Position target) override;

private:
	std::string name_;
	int         count_;
	int         range_;
};