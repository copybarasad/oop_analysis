#ifndef WEAPON_H
#define WEAPON_H

#include <memory>
enum class AttackMod {
	RangeCombat,
	CloseCombat
};

class Weapon {
public:
	Weapon(int level = 1);  // Новый конструктор по умолчанию — уровень 1
	Weapon(int damage, int attack_radius, AttackMod mode = AttackMod::RangeCombat);

	void ChangeAttackMod();
	int GetDamage() const;
	int GetAttackRadius() const;
	AttackMod GetAttackMode() const;

	// Новый метод — улучшить оружие (возвращает новое, более крутое)
	std::shared_ptr<Weapon> Upgrade() const;

	// Для сериализации
	int GetBaseDamage() const { return base_damage_; }
	int GetBaseRadius() const { return base_attack_radius_; }

private:
	int base_damage_;
	int base_attack_radius_;
	AttackMod attack_mode_;
};

#endif // WEAPON_H