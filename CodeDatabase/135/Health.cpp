#include "Health.h"
#include <algorithm>
#include <stdexcept>

Health::Health(int max_hp) : current_health(max_hp), max_health(max_hp) {
	if (max_hp <= 0) {
		throw std::invalid_argument("Максимальное здоровье должно быть положительным");
	}
}

void Health::reduceCurrentHealth(int damage) {
	current_health = std::max(0, current_health - damage);
}

void Health::restoreHealth() {
	current_health = max_health;
}

int Health::getCurrentHealth() const {
	return current_health;
}

int Health::getMaxHealth() const {
	return max_health;
}

void Health::increaseMaxHealth(int amount) {
	if (amount > 0) {
		max_health += amount;
		current_health += amount;
	}
}

bool Health::isDead() const {
	return current_health <= 0;
}

bool Health::isFullHealth() const {
	return current_health == max_health;
}

double Health::getHealthPercentage() const {
	if (max_health <= 0) {
		return 0.0;
	}
	return static_cast<double>(current_health) / static_cast<double>(max_health);
}

int Health::takeDamage(int amount) {
	if (amount < 0) {
		return 0;
	}
	
	int old_health = current_health;
	current_health = std::max(0, current_health - amount);
	int actual_damage = old_health - current_health;
	
	return actual_damage;
}

int Health::heal(int amount) {
	if (amount < 0) {
		return 0;
	}
	
	int old_health = current_health;
	current_health = std::min(max_health, current_health + amount);
	int actual_heal = current_health - old_health;
	
	return actual_heal;
}
