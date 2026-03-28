#pragma once
#include "Spells/Spell.h"
#include "DataTypes/SpellType.h"
#include "JsonLib/json.hpp"
#include <vector>


class PlayerHand
{
private:
	Point pos;
	int maxSize;
	std::vector<Spell*> spells;
	int upgradeCount = 0;
	int indexCurrent = -1;
public:
	PlayerHand(Point pos, int max = 5);
	PlayerHand(const PlayerHand& other);
	void to_json(nlohmann::json& js) const;
	void from_json(const nlohmann::json& js);
	void setPosition(const Point& point);
	void removeRandomSpells(int count);
	int getSpellsCount() const;
	int getMaxArmSize() const;
	std::string getSpellNameInTheHand() const;
	std::string getSpellInfo() const;
	const std::vector<Spell*> getSpells() const;
	void addSpell(SpellType type);
	void takeSpell(int index);
	void upgradeSpell(int count);
	bool useSpell(ISpellContext& context, const Point& destination);
	~PlayerHand();
};