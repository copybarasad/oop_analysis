#include "Enemy.h"

Enemy::Enemy() : Entity() {
	can_hit_on_walking_ = true;
}

Enemy::Enemy(int damage) : Enemy() {
	damage_ = std::min(100, std::max(damage, 10));
}

bool Enemy::IsFriendly() const {
	return false;
}

TokenEntity Enemy::serialise() const {
	TokenEntity token;
	token.type = "Enemy";
	token.hp = hp_;
	token.damage = damage_;
	token.can_move_on_cell = can_move_on_cell_;
	token.can_hit_on_walking = can_hit_on_walking_;
	token.not_move_next_turn = not_move_next_turn_;
	return token;
}