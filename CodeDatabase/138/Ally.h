#ifndef ALLY_H
#define ALLY_H

class Ally {
public:
	Ally(int x, int y, int health, int damage);
	
	int GetX() const;
	int GetY() const;
	int GetHealth() const;
	int GetDamage() const;
	bool IsAlive() const;
	void TakeDamage(int damage);
	
	void Move(class GameField& field);
	bool Attack(class GameField& field);

private:
	int positionX;
	int positionY;
	int currentHealth;
	int maximumHealth;
	int baseDamage;
	
	void validateInvariant() const;
};

#endif
