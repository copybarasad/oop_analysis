#include "Player.h"
#include <stdexcept>

Player::Player(int intel, int dex, int str, int base_attack, int hp,
               int melee_multi, int range_multi, size_t hand_size)
	: player_attributes(intel, dex, str),
	  player_attack(base_attack),
	  player_equipment(melee_multi, range_multi),
	  player_health(hp),
	  slowed(false),
	  spell_hand(hand_size),
	  spell_enhancer() {
	if (base_attack < 0) {
		throw std::invalid_argument("Базовая атака игрока не может быть отрицательной");
	}
	if (hp <= 0) {
		throw std::invalid_argument("Здоровье игрока должно быть положительным");
	}
}

EntityType Player::getType() const {
	return EntityType::PLAYER;
}

void Player::causeDamage(int damage) {
	player_health.reduceCurrentHealth(damage);
}

int Player::getDamage() const {
	return player_attack.getAttack() + player_equipment.getAttackBonus();
}

std::pair<int, int> Player::getHealth() const {
	return {player_health.getCurrentHealth(), player_health.getMaxHealth()};
}

bool Player::alive() const {
	return player_health.getCurrentHealth() > 0;
}

bool Player::melee() const {
	return player_equipment.isMelee();
}

void Player::setWeapon() {
	player_equipment.swapWeapon();
}

int Player::getInt() const {
	return player_attributes.getIntelligence();
}

int Player::getDex() const {
	return player_attributes.getDexterity();
}

int Player::getStr() const {
	return player_attributes.getStrength();
}

std::unique_ptr<IEntity> Player::clone() const {
	auto cloned = std::make_unique<Player>(
		player_attributes.getIntelligence(),
		player_attributes.getDexterity(),
		player_attributes.getStrength(),
		player_attack.getAttack(),
		player_health.getMaxHealth(),
		player_equipment.isMelee() ? player_equipment.getAttackBonus() : 0,
		player_equipment.isMelee() ? 0 : player_equipment.getAttackBonus(),
		spell_hand.getMaxSize()
	);
	
	cloned->setSlow(slowed);
	
	return cloned;
}

bool Player::isSlowed() const {
	return slowed;
}

void Player::setSlow(bool slow_state) {
	slowed = slow_state;
}

SpellHand& Player::getSpellHand() {
	return spell_hand;
}

const SpellHand& Player::getSpellHand() const {
	return spell_hand;
}

SpellEnhancer& Player::getSpellEnhancer() {
	return spell_enhancer;
}

const SpellEnhancer& Player::getSpellEnhancer() const {
	return spell_enhancer;
}

PlayerResources& Player::getResources() {
	return player_resources;
}

const PlayerResources& Player::getResources() const {
	return player_resources;
}

void Player::improveIntelligence(int amount) {
	player_attributes.increaseIntelligence(amount);
}

void Player::improveDexterity(int amount) {
	player_attributes.increaseDexterity(amount);
}

void Player::improveStrength(int amount) {
	player_attributes.increaseStrength(amount);
}

void Player::increaseMaxHealth(int amount) {
	player_health.increaseMaxHealth(amount);
}

void Player::restoreHealth() {
	player_health.restoreHealth();
}

int Player::calculateSpellPower() const {
	return player_attributes.calculateSpellPowerBonus();
}

const Attributes& Player::getAttributes() const {
	return player_attributes;
}

const Health& Player::getHealthStatus() const {
	return player_health;
}
