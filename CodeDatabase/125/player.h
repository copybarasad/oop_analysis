#pragma once
#include <string>
#include "position.h"
#include "player_hand.h"
#include "spell.h"

class EnemyManager;
class GameField;

enum class CombatMode { Melee, Ranged };

class Player {
public:
	Player();
	Player(const std::string& name, Position start);
	bool ProcessAction(char command, GameField& field, EnemyManager& em);
	void Attack(EnemyManager& em, GameField& field);
	bool CheckEnemyCollision(EnemyManager& em, Position pos);
	
	Position GetPosition() const;
	void SetPosition(const Position& pos);

	int GetCurrentDamage() const;
	void SwitchCombatMode();
	void TakeDamage(int dmg);
	void AddScore(int points);

	int GetHealth() const;
	int GetMaxHealth() const;
	int GetScore() const;
	CombatMode GetCombatMode() const;
	const std::string& GetName() const;
	bool IsAlive() const;

	const PlayerHand& GetHand() const;
	PlayerHand& GetHand();

	void ClearHand();

	bool BuySpell(SpellType type);

	void ResetHealth();                  
    void RemoveRandomHalfSpells();         

	void SetHealth(int health) { health_ = std::max(0, std::min(max_health_, health)); }
    void SetScore(int score) { score_ = score; }
    void SetCombatMode(CombatMode mode) { combat_mode_ = mode; }
    void SetMaxHealth(int max_health) { max_health_ = max_health; }

private:
	std::string name_;
	Position position_;
	int health_;
	int max_health_;
	int score_;
	int melee_damage_;
	int ranged_damage_;
	CombatMode combat_mode_;
	PlayerHand hand_;
};
