#pragma once
#include "Spell.h"


class DirectDamageSpell: public Spell
{
private:
	std::string name;
	int damage;
	int minRadius;
	int radius;
	int waitSteps;
	int leftSteps;
	int level = 1;
public:
	DirectDamageSpell(int damage = 20, int radius = 5, int waitSteps = 3, std::string name = "DirectDamage");
	void to_json(nlohmann::json& js) const override;
	void from_json(const nlohmann::json& js) override;
	std::string getName() const override;
	std::string getInfo() const override;
	int getLevel() const override;
	bool use(ISpellContext& context, const Point& usePos, const Point& destination) override;
	void upgrade() override;
	Spell* getCopy() const override;
};