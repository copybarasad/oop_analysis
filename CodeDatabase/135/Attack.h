#ifndef ATTACK_H
#define ATTACK_H

class Attack {
private:
	int attack;

public:
	Attack(int attack_value);
	
	bool setAttack(int new_attack);
	int getAttack() const;
	
	int calculateTotalDamage(int bonus) const;
};

#endif
