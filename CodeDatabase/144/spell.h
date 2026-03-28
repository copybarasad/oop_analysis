#ifndef SPELL_H
#define SPELL_H

#include <memory>
#include <string>

class SpellContext;

enum class SpellType {
	DIRECT_DAMAGE,
	AREA_DAMAGE,
	TRAP,
	SUMMON,
	ENHANCEMENT
};

class Spell {
public:
	virtual ~Spell() = default;
	
	virtual bool cast(SpellContext& context) = 0;
	virtual std::unique_ptr<Spell> clone() const = 0;
	virtual void enhance(int enhancementLevel) = 0;
	virtual void applyPermanentEnhancement(int enhancementLevel) = 0;
	virtual int getEnhancementLevel() const { return 0; }
	
	virtual std::string getName() const = 0;
	virtual std::string getDescription() const = 0;
	virtual int getManaCost() const = 0;
	virtual SpellType getSpellType() const = 0;

	virtual bool isEnhancement() const { return false; }
};

#endif