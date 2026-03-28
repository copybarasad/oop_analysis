#ifndef PLAYER_H
#define PLAYER_H

#include "GameTypes.h"
#include "PlayerHand.h"
#include <memory>

class GameField;
class Spell;

class Player {
public:
	Player(int health = 100, int meleeDamage = 20, int rangedDamage = 15, size_t handSize = 5);
	
	int GetHealth() const;
	int GetMaxHealth() const;
	bool IsAlive() const;
	void TakeDamage(int damage);
	void Heal(int amount);
	void SetHealth(int health);
	void UpgradeMaxHealth(int amount);
	void UpgradeMeleeDamage(int amount);
	void UpgradeRangedDamage(int amount);
	
	int GetCurrentDamage() const;
	CombatMode GetCombatMode() const;
	
	int GetScore() const;
	void AddScore(int points);
	
	bool CanMove() const;
	void ApplySlowEffect();
	void RemoveSlowEffect();
	
	void SwitchCombatMode();
	
	bool UseSpell(size_t index, GameField& field, int targetX, int targetY);
	void AddSpell(std::unique_ptr<Spell> spell);
	bool TryBuySpell();
	
	size_t GetSpellCount() const;
	std::string GetSpellNameAt(size_t index) const;
	Spell* GetSpellAt(size_t index);
	void RemoveSpellAt(size_t index);
	
	static const int SPELL_COST = 30;

private:
	int currentHealth;
	int maximumHealth;
	int meleeCombatDamage;
	int rangedCombatDamage;
	int currentScore;
	CombatMode currentCombatMode;
	bool isSlowedByCell;
	PlayerHand hand;
	int enhancementLevel;
	
	void validateInvariant() const;
};

#endif
