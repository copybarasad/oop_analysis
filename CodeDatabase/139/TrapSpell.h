#pragma once
#include "Spell.h"


class TrapSpell : public Spell
{
private:
	std::string name;
	int minDamage;
	int damage;
	int level = 1;
public:
	TrapSpell(int damage = 5, std::string name = "TrapSpell");
	void to_json(nlohmann::json& js) const override;
	void from_json(const nlohmann::json& js) override;
	std::string getName() const override;
	std::string getInfo() const override;
	int getLevel() const override;
	bool use(ISpellContext& context, const Point& usePos, const Point& destination) override;
	void upgrade() override;
	Spell* getCopy() const override;
};