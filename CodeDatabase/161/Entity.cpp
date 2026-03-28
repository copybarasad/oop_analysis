#include "Entity.h"

Entity::Entity()
	: hp_(100),
	  damage_(10),
	  can_move_on_cell_(true),
	  can_hit_on_walking_(false),
	  not_move_next_turn_(false) {}

int Entity::GetDamage() const {
	return damage_;
}

bool Entity::CauseDamage(int caused_damage) {
	hp_ -= caused_damage;
	return hp_ <= 0;
}

bool Entity::IsDead() const {
	return hp_ <= 0;
}

bool Entity::CanHit() const {
	return can_hit_on_walking_;
}

bool Entity::NotMoveNext() {
	if (not_move_next_turn_) {
		not_move_next_turn_ = false;
		return true;
	}
	return false;
}

bool Entity::IsStatic() const {
	return !can_move_on_cell_;
}

bool Entity::IsFriendly() const {
	return true;
}

void Entity::SetNotMoveNext(bool status) {
	not_move_next_turn_ = status;
}

int Entity::GetHP() const {
	return hp_;
}

