#pragma once
#include <string>

enum class Faction {
	PLAYER,
	ENEMY
};

class BasicBattler {
public:
	BasicBattler(int health, int damage, std::string name);
	virtual ~BasicBattler() = default;

	void hit(BasicBattler* target);
	virtual void takeDamage(int damage_dealt);

	bool isAlive();
	int getHealth() const;
	int getDamage() const;
	void setDamage(int new_damage);
	std::string getName() const;
	void setName(std::string new_name);

	virtual Faction getFaction() const = 0;
	bool isAlly(BasicBattler* other) const;

protected:
	void setHealth(int new_health);
	int health_;
	int damage_;
	std::string name_;
};