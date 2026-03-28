#ifndef TRAP_H
#define TRAP_H

class Trap {
public:
	Trap(int x, int y, int damage);
	
	int GetX() const;
	int GetY() const;
	int GetDamage() const;
	bool IsTriggered() const;
	void Trigger();

private:
	int positionX;
	int positionY;
	int damage;
	bool triggered;
};

#endif
