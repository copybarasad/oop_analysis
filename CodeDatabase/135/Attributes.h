#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

class Attributes {
private:
	int intelligence;
	int dexterity;
	int strength;

public:
	Attributes(int intel, int dex, int str);
	
	int getIntelligence() const;
	int getDexterity() const;
	int getStrength() const;
	
	void increaseIntelligence(int amount);
	void increaseDexterity(int amount);
	void increaseStrength(int amount);
	
	int calculateSpellPowerBonus() const;
	int calculatePhysicalDamageBonus() const;
	int calculateDodgeChance() const;
	bool canImprove() const;
};

#endif
