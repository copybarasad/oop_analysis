#include "Player.h"

Player::Player()
	: Entity(100, 15), attackMode(AttackMode::Melee), score(0), murders(0) {}

Player::Player(dataPlayer data) : Entity(data.health, data.damage), attackMode(data.mode), score(data.score), murders(0) {
	SetStunDuration(data.stunDuration);
}

Player::~Player() = default;

void Player::SwitchAttackMode() {
	attackMode = (attackMode == AttackMode::Melee) ? AttackMode::Ranged : AttackMode::Melee;
	switch (attackMode) {
	case AttackMode::Melee:
		damage = 15;
		break;
	case AttackMode::Ranged:
		damage = 10;
		break;
	}
}

void Player::AddScore(int s) {
	if (s>0){
		murders+=1;
	}
	score+=s;
}

AttackMode Player::GetAttackMode() {
    return attackMode;
}

int Player::GetScore() const {
	return score;
}

int Player::GetMurders(){
	return murders;
}