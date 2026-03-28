#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
	Enemy(int health = 50, int damage = 10);
	
	int GetHealth() const;
	int GetMaxHealth() const;
	bool IsAlive() const;
	void TakeDamage(int damage);
	
	int GetX() const;
	int GetY() const;
	void SetPosition(int x, int y);
	
	int GetCurrentDamage() const;
	int GetBaseDamage() const;

private:
	int currentHealth;
	int maximumHealth;
	int baseDamage;
	int positionX;
	int positionY;
	
	void validateInvariant() const;
};

#endif 
