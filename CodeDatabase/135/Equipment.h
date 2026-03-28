#ifndef EQUIPMENT_H
#define EQUIPMENT_H

enum class EquippedWeapon { MELEE, RANGE };

class Equipment {
private:
	EquippedWeapon equipped_weapon;
	int melee_weapon_multi;
	int range_weapon_multi;

public:
	Equipment(int melee_multi, int range_multi);
	
	void swapWeapon();
	int getAttackBonus() const;
	bool isMelee() const;
	EquippedWeapon getWeapon() const;
};

#endif 
