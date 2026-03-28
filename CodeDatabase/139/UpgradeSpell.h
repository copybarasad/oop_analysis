#pragma once
#include "Spell.h"
#include "DataTypes/SpellType.h"


class UpgradeSpell : public Spell
{
private:
	std::string name;
	int upgradeCount = 1;
	int level = 1;
public:
	UpgradeSpell(std::string name = "UpgradeSpell");
	void to_json(nlohmann::json& js) const override;
	void from_json(const nlohmann::json& js) override;
	std::string getName() const override;
	std::string getInfo() const override;
	int getLevel() const override;
	bool use(ISpellContext& context, const Point& usePos, const Point& destination) override;
	void upgrade() override;
	Spell* getCopy() const override;
};