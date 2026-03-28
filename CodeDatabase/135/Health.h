#ifndef HEALTH_H
#define HEALTH_H

class Health {
private:
	int current_health;
	int max_health;

public:
	Health(int max_hp);
	
	void reduceCurrentHealth(int damage);
	void restoreHealth();
	int getCurrentHealth() const;
	int getMaxHealth() const;
	
	void increaseMaxHealth(int amount);
	
	bool isDead() const;
	bool isFullHealth() const;
	double getHealthPercentage() const;
	int takeDamage(int amount);
	int heal(int amount);
};

#endif 
