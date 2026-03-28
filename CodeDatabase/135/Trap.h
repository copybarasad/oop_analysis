#ifndef TRAP_H
#define TRAP_H

class Trap {
private:
	int damage;
	bool active;

public:
	Trap(int trap_damage);
	
	int getDamage() const;
	bool isActive() const;
	void trigger();
};

#endif
