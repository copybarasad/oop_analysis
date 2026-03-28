#include "player.h"
#include "spell_factory.h"
#include <string>
#include <iostream>

Player::Player(int health, int damage, std::string name, int score, int defense, size_t max_hand_size)
	:BasicBattler(health, damage, name),
	hand_(max_hand_size),
	enemies_defeated_counter_(0)
{
	if (score >= 0)
		score_ = score;
	else {
		std::cout << "Количество очков должно быть положительным!" << std::endl;
		std::cout << "По умолчанию оно будет равно 0." << std::endl;
		score_ = 0;
	}

	if (defense > 0)
		defense_ = defense;
	else {
		std::cout << "Значение защиты должно быть положительным!" << std::endl;
		std::cout << "По умолчанию оно будет равно 1." << std::endl;
		defense_ = 1;
	}
}

void Player::takeDamage(int damage_dealt) {
	int actual_damage = std::max(damage_dealt - defense_, 1);

	std::cout << "Защита игрока " << name_ << " (" << defense_
		<< ") блокирует часть урона!" << std::endl;

	int old_health = health_;
	setHealth(health_ - actual_damage);

	std::cout << "Здоровье игрока " << name_
		<< " уменьшилось с " << old_health
		<< " до " << health_ << std::endl;
}
int Player::getScore() const{
	return score_;
}
int Player::getDefense() const{
	return defense_;
}

void Player::addScore(int points) {
	score_ += points;
}

void Player::processKillReward() {
	const int accrued_points = 10;
	addScore(accrued_points);
	std::cout << "\nИгрок " << getName() << " победил врага и получает "
		<< accrued_points << " очков! Текущий счет: "
		<< getScore() << std::endl;

	enemies_defeated_counter_++;
	std::cout << "Всего врагов повержено: " << enemies_defeated_counter_ << std::endl;

	const int enemies_per_spell = 2;
	if (enemies_defeated_counter_ > 0 && enemies_defeated_counter_ % enemies_per_spell == 0) {
		hand_.addSpell(SpellFactory::createRandomSpell());
	}
}

PlayerHand& Player::getHand() {
	return hand_;
}

void Player::showSpells() const {
	hand_.show();
}

SpellCastResult Player::useSpell(size_t index, PlayingField& field) {
	return hand_.useSpell(index, this, field);
}

Faction Player::getFaction() const {
	return Faction::PLAYER;
}

void Player::heal(int amount) {
	if (amount > 0) {
		setHealth(health_ + amount);
	}
	if (health_ > 100) {
		setHealth(100);
	}
}