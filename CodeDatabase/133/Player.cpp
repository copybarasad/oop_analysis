#include "Player.h"
#include <algorithm>
#include "SpellFactory.h"

Player::Player(int initialHealth, int meleeDmg, int rangedDmg, int handSize)
	: health(initialHealth),
	maxHealth(initialHealth),
	meleeDamage(meleeDmg),
	rangedDamage(rangedDmg),
	score(0),
	currentAttackType(AttackType::MELEE),
	isSlowed(false),
	spellHand(handSize),
	enemiesDefeated(0),
	enhancementStack(0) {}

void Player::takeDamage(int damage) {
	health -= damage;
	if (health < 0) {
		health = 0;
	}
}

void Player::addScore(int points) {
	score += points;
	incrementEnemiesDefeated();
}

void Player::setScore(int newScore) {
	score = std::max(0, newScore);
}

void Player::switchAttackType() {
	currentAttackType = (currentAttackType == AttackType::MELEE) ? AttackType::RANGED : AttackType::MELEE;
}

void Player::setAttackType(AttackType type) {
	currentAttackType = type;
}

void Player::setSlowed(bool slowed) {
	isSlowed = slowed;
}

void Player::heal(int amount) {
	health += amount;
	if (health > maxHealth) {
		health = maxHealth;
	}
}

void Player::restoreFullHealth() {
	health = maxHealth;
}

void Player::increaseMaxHealth(int delta) {
	maxHealth = std::max(1, maxHealth + delta);
	restoreFullHealth();
}

void Player::increaseMeleeDamage(int delta) {
	meleeDamage = std::max(0, meleeDamage + delta);
}

void Player::increaseRangedDamage(int delta) {
	rangedDamage = std::max(0, rangedDamage + delta);
}

void Player::setEnemiesDefeated(int defeated) {
	enemiesDefeated = std::max(0, defeated);
}

void Player::setEnhancementStack(int stack) {
	enhancementStack = std::max(0, stack);
}

bool Player::canBuySpell() const {
	return score >= 50 && spellHand.getSize() < spellHand.getMaxSize();
}

void Player::buySpell(std::unique_ptr<Spell> spell) {
	if (canBuySpell()) {
		score -= 50;
		spellHand.addSpell(std::move(spell));
	}
}

PlayerState Player::createState() const {
	PlayerState state{};
	state.health = health;
	state.maxHealth = maxHealth;
	state.meleeDamage = meleeDamage;
	state.rangedDamage = rangedDamage;
	state.score = score;
	state.attackType = currentAttackType;
	state.slowed = isSlowed;
	state.enemiesDefeated = enemiesDefeated;
	state.enhancementStack = enhancementStack;
	state.spellHandCapacity = spellHand.getMaxSize();
	state.spellIds = spellHand.getSpellIds();
	return state;
}

void Player::applyState(const PlayerState& state) {
	health = state.health;
	maxHealth = state.maxHealth;
	meleeDamage = state.meleeDamage;
	rangedDamage = state.rangedDamage;
	score = state.score;
	currentAttackType = state.attackType;
	isSlowed = state.slowed;
	enemiesDefeated = state.enemiesDefeated;
	enhancementStack = state.enhancementStack;
	spellHand = SpellHand(state.spellHandCapacity, false);

	std::vector<std::unique_ptr<Spell>> restored;
	restored.reserve(state.spellIds.size());
	for (SpellId id : state.spellIds) {
		restored.push_back(SpellFactory::create(id));
	}
	spellHand.replaceSpells(std::move(restored));
}