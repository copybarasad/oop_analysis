#ifndef UNIT_H
#define UNIT_H

#include <string>

// Base class for all game characters (Player and Enemy)
// Contains common characteristics: HP, damage, and score
class Unit {
protected:
	int hp;
	int damage;
	int score;

public:
	Unit(int hp, int damage, int score = 0);
	virtual ~Unit() = default;

	// Getters
	int getHP() const;
	int getDamage() const;
	int getScore() const;

	// Modifiers
	void takeDamage(int amount);
	void addScore(int points);

	// Status
	bool isAlive() const;
};

#endif // UNIT_H
