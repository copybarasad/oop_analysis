#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>
#include "SpellHand.h"
#include "GameConstants.h"

enum class AttackType {
	MELEE,
	RANGED
};

class Spell;

struct PlayerState {
	int health;
	int maxHealth;
	int meleeDamage;
	int rangedDamage;
	int score;
	AttackType attackType;
	bool slowed;
	int enemiesDefeated;
	int enhancementStack;
	int spellHandCapacity;
	std::vector<SpellId> spellIds;
};

class Player {
private:
	int health;
	int maxHealth;
	int meleeDamage;
	int rangedDamage;
	int score;
	AttackType currentAttackType;
	bool isSlowed;
	SpellHand spellHand;
	int enemiesDefeated;
	int enhancementStack;

public:
	Player(int initialHealth = GameConstants::DEFAULT_PLAYER_HEALTH,
		int meleeDmg = GameConstants::DEFAULT_PLAYER_MELEE_DAMAGE,
		int rangedDmg = GameConstants::DEFAULT_PLAYER_RANGED_DAMAGE,
		int handSize = 5);

	int getHealth() const { return health; }
	int getMaxHealth() const { return maxHealth; }
	int getMeleeDamage() const { return meleeDamage; }
	int getRangedDamage() const { return rangedDamage; }
	int getScore() const { return score; }
	AttackType getAttackType() const { return currentAttackType; }
	int getDamage() const { return (currentAttackType == AttackType::MELEE) ? meleeDamage : rangedDamage; }
	bool getIsSlowed() const { return isSlowed; }
	SpellHand& getSpellHand() { return spellHand; }
	const SpellHand& getSpellHand() const { return spellHand; }
	int getEnemiesDefeated() const { return enemiesDefeated; }
	int getEnhancementStack() const { return enhancementStack; }

	void takeDamage(int damage);
	void addScore(int points);
	void setScore(int newScore);
	void switchAttackType();
	void setAttackType(AttackType type);
	void setSlowed(bool slowed);
	void heal(int amount);
	void restoreFullHealth();
	void increaseMaxHealth(int delta);
	void increaseMeleeDamage(int delta);
	void increaseRangedDamage(int delta);
	void incrementEnemiesDefeated() { enemiesDefeated++; }
	void setEnemiesDefeated(int defeated);
	void accumulateEnhancement() { enhancementStack++; }
	void resetEnhancement() { enhancementStack = 0; }
	void setEnhancementStack(int stack);

	bool isAlive() const { return health > 0; }
	bool canBuySpell() const;
	void buySpell(std::unique_ptr<Spell> spell);
	void resetEnemiesDefeatedCounter() { enemiesDefeated = 0; }

	PlayerState createState() const;
	void applyState(const PlayerState& state);
};

#endif