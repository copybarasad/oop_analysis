#ifndef SPELL_H
#define SPELL_H

#include <string>

class GameField;

class Spell {
public:
	virtual ~Spell() = default;
	
	virtual bool Cast(GameField& field, int targetX, int targetY) = 0;
	virtual std::string GetName() const = 0;
	virtual void Enhance() = 0;
	virtual bool IsEnhancement() const { return false; }

protected:
	int baseDamage;
	int currentDamage;
	int baseRadius;
	int currentRadius;
	
	Spell(int damage, int radius)
		: baseDamage(damage), currentDamage(damage),
		  baseRadius(radius), currentRadius(radius) {}
	
	Spell() : baseDamage(0), currentDamage(0), baseRadius(0), currentRadius(0) {}
	
	bool IsInRange(GameField& field, int targetX, int targetY) const;
	
	virtual void EnhanceBase() {
		currentDamage = static_cast<int>(baseDamage * 1.1);
		currentRadius = static_cast<int>(baseRadius * 1.1);
	}
};

#endif
