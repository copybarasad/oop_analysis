#include "basic_battler.h"
#include <iostream>
#include <string>
#include <algorithm>

BasicBattler::BasicBattler(int health, int damage, std::string name){
	health_ = health;

	if(damage > 0)
		damage_ = damage;
	else {
		std::cout << "Урон должен быть положительным!" << std::endl;
		std::cout << "По умолчанию он будет равен 1." << std::endl;
		damage_ = 1;
	}
	
	if (name.empty()) {
		std::cout << "Имя бойца не может быть пустым!" << std::endl;
		std::cout << "По умолчанию будет присвоено имя 'Unknown'." << std::endl;
		name_ = "Unknown";
	}
	else {
		name_ = name;
	}
}

void BasicBattler::hit(BasicBattler* target) {
	std::cout << "Боец " << name_ << " атакует бойца " << target->getName()
		<< " с силой " << damage_ << "." << std::endl;
	target->takeDamage(damage_);
}

void BasicBattler::takeDamage(int damage_dealt) {
	int old_health = health_;
	setHealth(health_ - damage_dealt);
	std::cout << "Здоровье бойца " << name_
		<< " уменьшилось с " << old_health
		<< " до " << health_ << std::endl;
}

bool BasicBattler::isAlive() {
	return health_ > 0;
}

int BasicBattler::getHealth() const{
	return health_;
}
void BasicBattler::setHealth(int new_health){
	health_ = std::max(new_health, 0);
}
int BasicBattler::getDamage() const{
	return damage_;
}
void BasicBattler::setDamage(int new_damage) {
	damage_ = std::max(new_damage, 0);
}
std::string BasicBattler::getName() const{
	return name_;
}
void BasicBattler::setName(std::string new_name) {
	name_ = new_name;
}

bool BasicBattler::isAlly(BasicBattler* other) const {
	if (!other) {
		return false;
	}
	return this->getFaction() == other->getFaction();
}
