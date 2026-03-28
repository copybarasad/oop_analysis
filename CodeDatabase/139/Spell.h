#pragma once
#include "Interfaces/ISpellContext.h"
#include "DataTypes/SpellType.h"
#include "JsonLib/json.hpp"
#include <string>



class Spell
{
public:
	virtual void to_json(nlohmann::json& js) const = 0;
	virtual void from_json(const nlohmann::json& js) = 0;
	virtual std::string getName() const = 0;
	virtual std::string getInfo() const = 0;
	virtual int getLevel() const = 0;
	virtual bool use(ISpellContext& context, const Point& usePos, const Point& destination) = 0;
	virtual void upgrade() = 0;
	virtual Spell* getCopy() const = 0;
};