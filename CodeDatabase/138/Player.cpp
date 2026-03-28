#include "Player.h"
#include "PlayerHand.h"
#include "GameField.h"
#include "Spell.h"
#include "SpellFactory.h"
#include <stdexcept>

Player::Player(int health, int meleeDamage, int rangedDamage, size_t handSize) 
	: currentHealth(health), maximumHealth(health), meleeCombatDamage(meleeDamage), 
	  rangedCombatDamage(rangedDamage), currentScore(0), currentCombatMode(CombatMode::MELEE), 
	  isSlowedByCell(false), hand(handSize), enhancementLevel(0) {
	validateInvariant();
}

int Player::GetHealth() const {
	return currentHealth;
}

int Player::GetMaxHealth() const {
	return maximumHealth;
}

bool Player::IsAlive() const {
	return currentHealth > 0;
}

void Player::TakeDamage(int damage) {
	if (damage < 0) {
		throw std::invalid_argument("Damage cannot be negative");
	}
	currentHealth -= damage;
	if (currentHealth < 0) {
		currentHealth = 0;
	}
	validateInvariant();
}

void Player::Heal(int amount) {
	if (amount < 0) {
		throw std::invalid_argument("Heal amount cannot be negative");
	}
	currentHealth += amount;
	if (currentHealth > maximumHealth) {
		currentHealth = maximumHealth;
	}
	validateInvariant();
}

void Player::SetHealth(int health) {
	if (health < 0 || health > maximumHealth) {
		throw std::invalid_argument("Invalid health value");
	}
	currentHealth = health;
	validateInvariant();
}

void Player::UpgradeMaxHealth(int amount) {
	if (amount < 0) {
		throw std::invalid_argument("Upgrade amount cannot be negative");
	}
	maximumHealth += amount;
	currentHealth += amount;
	validateInvariant();
}

void Player::UpgradeMeleeDamage(int amount) {
	if (amount < 0) {
		throw std::invalid_argument("Upgrade amount cannot be negative");
	}
	meleeCombatDamage += amount;
	validateInvariant();
}

void Player::UpgradeRangedDamage(int amount) {
	if (amount < 0) {
		throw std::invalid_argument("Upgrade amount cannot be negative");
	}
	rangedCombatDamage += amount;
	validateInvariant();
}

int Player::GetCurrentDamage() const {
	return (currentCombatMode == CombatMode::MELEE) ? 
		meleeCombatDamage : rangedCombatDamage;
}

CombatMode Player::GetCombatMode() const {
	return currentCombatMode;
}

int Player::GetScore() const {
	return currentScore;
}

void Player::AddScore(int points) {
	if (points < 0) {
		throw std::invalid_argument("Score points cannot be negative");
	}
	currentScore += points;
}

bool Player::CanMove() const {
	return !isSlowedByCell;
}

void Player::ApplySlowEffect() {
	isSlowedByCell = true;
}

void Player::RemoveSlowEffect() {
	isSlowedByCell = false;
}

void Player::SwitchCombatMode() {
	currentCombatMode = (currentCombatMode == CombatMode::MELEE) ? 
		CombatMode::RANGED : CombatMode::MELEE;
}

bool Player::UseSpell(size_t index, GameField& field, int targetX, int targetY) {
	Spell* spell = hand.GetSpellAt(index);
	if (!spell) {
		return false;
	}
	
	for (int i = 0; i < enhancementLevel; ++i) {
		spell->Enhance();
	}
	
	bool result = hand.UseSpell(index, field, targetX, targetY);
	
	if (result) {
		if (spell->IsEnhancement()) {
			enhancementLevel++;
		} else {
			enhancementLevel = 0;
		}
	}
	
	return result;
}

void Player::AddSpell(std::unique_ptr<Spell> spell) {
	hand.AddSpell(std::move(spell));
}

bool Player::TryBuySpell() {
	if (currentScore < SPELL_COST) {
		return false;
	}
	if (hand.IsFull()) {
		return false;
	}
	currentScore -= SPELL_COST;
	return true;
}

size_t Player::GetSpellCount() const {
	return hand.GetSpellCount();
}

std::string Player::GetSpellNameAt(size_t index) const {
	return hand.GetSpellName(index);
}

Spell* Player::GetSpellAt(size_t index) {
	return hand.GetSpellAt(index);
}

void Player::RemoveSpellAt(size_t index) {
	hand.RemoveSpell(index);
}

void Player::validateInvariant() const {
	if (currentHealth < 0 || currentHealth > maximumHealth) {
		throw std::runtime_error("Player health invariant violated");
	}
	if (maximumHealth <= 0) {
		throw std::runtime_error("Player maximum health must be positive");
	}
	if (meleeCombatDamage < 0 || rangedCombatDamage < 0) {
		throw std::runtime_error("Player damage values cannot be negative");
	}
	if (currentScore < 0) {
		throw std::runtime_error("Player score cannot be negative");
	}
}

