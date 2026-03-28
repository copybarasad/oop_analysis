#include "unit.h"

Unit::Unit(int health, int damage): _health(health), _damage(damage) {
	if (_health < 0) _health = 0;
	if (_damage < 0) _damage = 0;
}

int Unit::get_health() const{
	return _health;
}

int Unit::get_damage() const{
	return _damage;
}

bool Unit::isAlive() const{
	return _health > 0;
}

Position Unit::getPosition() const {
    return _position;
}

void Unit::setPosition(const Position& newPosition) {
    _position = newPosition;
}

void Unit::freeze() {
	if (_isFrozen) _isFrozen = false;
	else _isFrozen = true;
}

bool Unit::isFrozen() const {
    return _isFrozen;
}


CellType Unit::get_type() const {
    return _type;
}

void Unit::takeDamage(int value){
	if (value < 0) return; // отрицательный урон игнорируем
	if (value >= _health) {
		_health = 0;
	} else {
		_health -= value;
	}
}

void Unit::change_health(int value){
	_health = value;
}

void Unit::change_damage(int value){
	_damage = value;
}
