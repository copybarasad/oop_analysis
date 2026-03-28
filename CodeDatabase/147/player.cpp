#include "player.h"

Player::Player(int health, int damage): Unit(health, damage) {
	_type = CellType::PLAYER;
	_isFrozen = false;
}

void Player::addScore(int value){
	if (value < 0) return;
	_score += value;
}

int Player::get_score() const {
	return _score;
}

void Player::set_score(int value){
	if (value < 0) return;
	_score = value;
}

void Player::change_attack_version() {
	if (attack_version == 'M'){
		attack_version = 'F';
		_damage /= 2;
	} 
	else{
		attack_version = 'M';
		_damage *= 2;
	} 
}

char Player::get_attack_version() const {
	return attack_version;
}
