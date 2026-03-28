#pragma once
#include "Spell.h"


class BuffSpell : public Spell 
{
public:
	BuffSpell(std::string name);

	std::string GetName() const override;
	bool   Apply(Player& caster, Game& game, Position target) override;
private:
	std::string name_;
};