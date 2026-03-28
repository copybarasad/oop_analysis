#pragma once
#include <string>
#include <memory>
#include "Coords.hpp"

class SpellManager;

class ISpell {
public:
	virtual ~ISpell() = default;
	virtual bool cast(SpellManager& spellManager, const Coords& casterPos, const Coords& target) = 0;
	virtual int getRange() const noexcept = 0;
	virtual int getDamage() const noexcept = 0;
	virtual std::string getName() const = 0;
	virtual std::string getDescription() const = 0;
	virtual std::unique_ptr<ISpell> clone() const = 0;
};
