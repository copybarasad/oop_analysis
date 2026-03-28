#include <algorithm>

#include "Weapon.h"

Weapon::Weapon(int level)
	: Weapon(40 + level * 15, 2, AttackMod::RangeCombat) {}

Weapon::Weapon(int damage, int attack_radius, AttackMod mode)
	: base_damage_(damage > 0 ? damage : 50),
	  base_attack_radius_(attack_radius > 0 ? attack_radius : 2),
	  attack_mode_(mode) {}

std::shared_ptr<Weapon> Weapon::Upgrade() const {
	return std::make_shared<Weapon>(
		base_damage_ + 20,
		base_attack_radius_,
		attack_mode_
	);
}


void Weapon::ChangeAttackMod() {
	if (attack_mode_ == AttackMod::RangeCombat) {
		attack_mode_ = AttackMod::CloseCombat;
	} else {
		attack_mode_ = AttackMod::RangeCombat;
	}
}

int Weapon::GetDamage() const {
	if (attack_mode_ == AttackMod::CloseCombat) {
		return base_damage_ * 2;
	}
	return base_damage_;
}

int Weapon::GetAttackRadius() const {
	if (attack_mode_ == AttackMod::CloseCombat) {
		return std::max(base_attack_radius_ / 2, 1);
	}
	return base_attack_radius_;
}

AttackMod Weapon::GetAttackMode() const {
	return attack_mode_;
}