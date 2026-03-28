#pragma once
#include "Spell.h"


class AllySpell : public Spell
{
	std::string name;
	int health;
	int damage;
	int radius = 2;
	int alliesCount = 1;
	int level = 1;
public:
	AllySpell(int health = 30, int damage = 10, std::string name = "AllySpell");
	void to_json(nlohmann::json& js) const override;
	void from_json(const nlohmann::json& js) override;
	std::string getName() const override;
	std::string getInfo() const override;
	int getLevel() const override;
	bool use(ISpellContext& context, const Point& usePos, const Point& destination) override;
	void upgrade() override;
	Spell* getCopy() const override;
};