#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include "spell_context.h"
#include "spell_state.h"
#include <memory>
#include <vector>

class Spell;
class Game;

enum class CombatMode {
    MELEE,
    RANGED
};

class Player {
private:
    int maximumHealth;
    int currentHealth;
    int meleeAttackDamage;
    int rangedAttackDamage;
    int score;
    CombatMode currentCombatMode;
    bool isMovementSlowed;
    int rangedAttackRange;
    int positionX;
    int positionY;
    Hand spellHand;
    int currentMana;
    int maximumMana;
    int currentEnhancementLevel;

public:
	struct Snapshot {
		int maximumHealth = 0;
		int currentHealth = 0;
		int meleeAttackDamage = 0;
		int rangedAttackDamage = 0;
		int score = 0;
		CombatMode combatMode = CombatMode::MELEE;
		bool slowed = false;
		int rangedAttackRange = 0;
		int positionX = -1;
		int positionY = -1;
		int currentMana = 0;
		int maximumMana = 0;
		int currentEnhancementLevel = 0;
		std::vector<SpellState> handSpells;
	};

    Player(int health = 1000, int meleeDamage = 15, int rangedDamage = 8, 
           int rangedRange = 3, int handSize = 5, int mana = 3000);
    
    int getX() const;
    int getY() const;
    void setPosition(int newX, int newY);
    
    int getHealth() const;
    int getMaxHealth() const;
	int getMeleeAttackDamageValue() const;
	int getRangedAttackDamageValue() const;
    int getDamage() const;
    int getScore() const;
    CombatMode getCombatMode() const;
    bool isSlowed() const;
    int getRangedAttackRange() const;
    
    void takeDamage(int damageAmount);
    void addScore(int points);
    void switchCombatMode();
    void setSlowed(bool slowedStatus);
	void setMaximumHealth(int value);
	void setCurrentHealth(int value);
	void setMeleeAttackDamage(int value);
	void setRangedAttackDamage(int value);
	void setScoreValue(int value);
	void setRangedAttackRange(int value);
	void setManaValues(int current, int max);
	void setCombatMode(CombatMode mode);
    void heal(int healAmount);
    bool isAlive() const;
    
    Hand& getHand();
    const Hand& getHand() const;
    int getMana() const;
    int getMaxMana() const;
    bool consumeMana(int manaAmount);
    void restoreMana(int manaAmount);
    
    void applyEnhancement(int enhancementLevel);
    void clearEnhancements();
    int getCurrentEnhancementLevel() const;
    bool hasActiveEnhancement() const;
	void setEnhancementLevel(int level);
	bool castSpellWithEnhancements(int spellIndex, SpellContext& context);
	Snapshot createSnapshot() const;
	void applySnapshot(const Snapshot& snapshot);
};

#endif