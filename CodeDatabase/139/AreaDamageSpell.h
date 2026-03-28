#pragma once
#include "Spell.h"


class AreaDamageSpell: public Spell
{
	std::string name;
	int damage;
	int radius;
	int area = 2;
	int level = 1;
public:
	AreaDamageSpell(int damage = 10, int radius = 3, std::string name = "AreaDamage");
	void to_json(nlohmann::json& js) const override;
	void from_json(const nlohmann::json& js) override;
	std::string getName() const override;
	std::string getInfo() const override;
	int getLevel() const override;
	bool use(ISpellContext& context, const Point& usePos, const Point& destination) override;
	void upgrade() override;
	Spell* getCopy() const override;
};