#include "Ally.h"


Ally::Ally() : Entity() {
	can_hit_on_walking_ = true;
}

Ally::Ally(int damage) : Ally() {
	damage_ = std::min(100, std::max(damage, 10));
}

bool Ally::IsFriendly() const {
	return true;
}

TokenEntity Ally::serialise() const {
	TokenEntity token;
	token.type = "Ally";
	token.hp = hp_;
	token.damage = damage_;
	token.can_move_on_cell = can_move_on_cell_;
	token.can_hit_on_walking = can_hit_on_walking_;
	token.not_move_next_turn = not_move_next_turn_;
	return token;
}