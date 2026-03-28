#include "Entity.h"
#include "iostream"

Entity::Entity(int healthPoints, int attackDamage)
	: damage(attackDamage), health(healthPoints), stunDuration(0) {}
	

Entity::~Entity() = default;

void Entity::TakeDamage(int amount) {
	std::cout << "aaaaaaaaaaaa\n";
	health -= amount;
}

int Entity::GetDamage() const {
	return damage;
}

bool Entity::IsAlive() const {
	return health > 0;
}

int Entity::GetHealth() const {
	return health;
}

void Entity::SetStunDuration(int duration) {
	stunDuration = duration;
}

int Entity::GetStunDuration() const {
	return stunDuration;
}

bool Entity::IsStunned() const {
	return stunDuration > 0;
}

void Entity::DecreaseStunDuration() {
	if (stunDuration > 0) stunDuration--;
}
