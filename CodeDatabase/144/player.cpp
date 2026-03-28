#include "player.h"
#include "spell.h"
#include <algorithm>
#include <iostream>

Player::Player(int health, int meleeDamage, int rangedDamage, int rangedRange, int handSize, int mana)
	: maximumHealth(health), currentHealth(health), meleeAttackDamage(meleeDamage), 
	  rangedAttackDamage(rangedDamage), score(0), currentCombatMode(CombatMode::MELEE), 
	  isMovementSlowed(false), rangedAttackRange(rangedRange), positionX(-1), positionY(-1),
	  spellHand(handSize), currentMana(mana), maximumMana(mana), currentEnhancementLevel(0) {}

int Player::getX() const {
	return positionX;
}

int Player::getY() const {
	return positionY;
}

void Player::setPosition(int newX, int newY) {
	positionX = newX;
	positionY = newY;
}

int Player::getHealth() const {
	return currentHealth;
}

int Player::getMaxHealth() const {
	return maximumHealth;
}

int Player::getMeleeAttackDamageValue() const {
	return meleeAttackDamage;
}

int Player::getRangedAttackDamageValue() const {
	return rangedAttackDamage;
}

int Player::getDamage() const {
	return (currentCombatMode == CombatMode::MELEE) ? meleeAttackDamage : rangedAttackDamage;
}

int Player::getScore() const {
	return score;
}

CombatMode Player::getCombatMode() const {
	return currentCombatMode;
}

bool Player::isSlowed() const {
	return isMovementSlowed;
}

int Player::getRangedAttackRange() const {
	return rangedAttackRange;
}

void Player::takeDamage(int damageAmount) {
	currentHealth = std::max(0, currentHealth - damageAmount);
}

void Player::addScore(int points) {
	score += points;
}

void Player::switchCombatMode() {
	currentCombatMode = (currentCombatMode == CombatMode::MELEE) ? 
					   CombatMode::RANGED : CombatMode::MELEE;
}

void Player::setSlowed(bool slowedStatus) {
	isMovementSlowed = slowedStatus;
}

void Player::setMaximumHealth(int value) {
	maximumHealth = value;
}

void Player::setCurrentHealth(int value) {
	currentHealth = std::max(0, std::min(value, maximumHealth));
}

void Player::setMeleeAttackDamage(int value) {
	meleeAttackDamage = value;
}

void Player::setRangedAttackDamage(int value) {
	rangedAttackDamage = value;
}

void Player::setScoreValue(int value) {
	score = value;
}

void Player::setRangedAttackRange(int value) {
	rangedAttackRange = value;
}

void Player::setManaValues(int current, int max) {
	maximumMana = max;
	currentMana = std::max(0, std::min(current, maximumMana));
}

void Player::setCombatMode(CombatMode mode) {
	currentCombatMode = mode;
}

void Player::heal(int healAmount) {
	currentHealth = std::min(maximumHealth, currentHealth + healAmount);
}

bool Player::isAlive() const {
	return currentHealth > 0;
}

Hand& Player::getHand() {
	return spellHand;
}

const Hand& Player::getHand() const {
	return spellHand;
}

int Player::getMana() const {
	return currentMana;
}

int Player::getMaxMana() const {
	return maximumMana;
}

bool Player::consumeMana(int manaAmount) {
	if (currentMana >= manaAmount) {
		currentMana -= manaAmount;
		return true;
	}
	return false;
}

void Player::restoreMana(int manaAmount) {
	currentMana = std::min(maximumMana, currentMana + manaAmount);
}

void Player::applyEnhancement(int enhancementLevel) {
	currentEnhancementLevel += enhancementLevel;
}

void Player::clearEnhancements() {
	currentEnhancementLevel = 0;
}

int Player::getCurrentEnhancementLevel() const {
	return currentEnhancementLevel;
}

bool Player::hasActiveEnhancement() const {
	return currentEnhancementLevel > 0;
}

void Player::setEnhancementLevel(int level) {
	currentEnhancementLevel = level;
}

bool Player::castSpellWithEnhancements(int spellIndex, SpellContext& context) {
	if (spellIndex < 0 || spellIndex >= spellHand.getSpellCount()) {
		return false;
	}
	
	auto& spells = spellHand.getSpells();
	if (spellIndex < spells.size()) {
		Spell* spell = spells[spellIndex].get();
		
		if (spell->isEnhancement()) {
			bool result = spellHand.castSpell(spellIndex, context);
			return result;
		} else {
			if (currentEnhancementLevel > 0) {
				spell->enhance(currentEnhancementLevel);
				std::cout << "Applying " << currentEnhancementLevel 
						  << " enhancement levels to " << spell->getName() << " spell!\n";
			}
			
			bool result = spellHand.castSpell(spellIndex, context);
			clearEnhancements();
			return result;
		}
	}
	
	return false;
}

Player::Snapshot Player::createSnapshot() const {
	Snapshot snapshot;
	snapshot.maximumHealth = maximumHealth;
	snapshot.currentHealth = currentHealth;
	snapshot.meleeAttackDamage = meleeAttackDamage;
	snapshot.rangedAttackDamage = rangedAttackDamage;
	snapshot.score = score;
	snapshot.combatMode = currentCombatMode;
	snapshot.slowed = isMovementSlowed;
	snapshot.rangedAttackRange = rangedAttackRange;
	snapshot.positionX = positionX;
	snapshot.positionY = positionY;
	snapshot.currentMana = currentMana;
	snapshot.maximumMana = maximumMana;
	snapshot.currentEnhancementLevel = currentEnhancementLevel;
	
	SpellSerializer serializer;
	const auto& spells = spellHand.getSpells();
	snapshot.handSpells.reserve(spells.size());
	for (const auto& spell : spells) {
		snapshot.handSpells.push_back(serializer.serialize(*spell));
	}
	
	return snapshot;
}

void Player::applySnapshot(const Snapshot& snapshot) {
	maximumHealth = snapshot.maximumHealth;
	currentHealth = std::max(0, std::min(snapshot.currentHealth, snapshot.maximumHealth));
	meleeAttackDamage = snapshot.meleeAttackDamage;
	rangedAttackDamage = snapshot.rangedAttackDamage;
	score = snapshot.score;
	currentCombatMode = snapshot.combatMode;
	isMovementSlowed = snapshot.slowed;
	rangedAttackRange = snapshot.rangedAttackRange;
	positionX = snapshot.positionX;
	positionY = snapshot.positionY;
	currentMana = snapshot.currentMana;
	maximumMana = snapshot.maximumMana;
	currentEnhancementLevel = snapshot.currentEnhancementLevel;
	
	auto& spells = spellHand.getSpells();
	spells.clear();
	
	SpellSerializer serializer;
	for (const auto& spellState : snapshot.handSpells) {
		spells.push_back(serializer.deserialize(spellState));
	}
}