#include "Equipment.h"
#include <stdexcept>

Equipment::Equipment(int melee_multi, int range_multi) 
	: equipped_weapon(EquippedWeapon::MELEE), 
	  melee_weapon_multi(melee_multi), 
	  range_weapon_multi(range_multi) {
	if (melee_multi < 0 || range_multi < 0) {
		throw std::invalid_argument("Weapon multipliers cannot be negative");
	}
}

void Equipment::swapWeapon() {
	if (equipped_weapon == EquippedWeapon::MELEE) {
		equipped_weapon = EquippedWeapon::RANGE;
	} else {
		equipped_weapon = EquippedWeapon::MELEE;
	}
}

int Equipment::getAttackBonus() const {
	if (equipped_weapon == EquippedWeapon::MELEE) {
		return melee_weapon_multi;
	} else {
		return range_weapon_multi;
	}
}

bool Equipment::isMelee() const {
	return equipped_weapon == EquippedWeapon::MELEE;
}

EquippedWeapon Equipment::getWeapon() const {
	return equipped_weapon;
}
