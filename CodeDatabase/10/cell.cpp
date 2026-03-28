#include "cell.h"

Cell::Cell() : type(Type::EMPTY) {};

Type Cell::getType() const { return type; }

bool Cell::hasEnemy() const { return type == Type::ENEMY;}
bool Cell::hasPlayer() const { return type == Type::PLAYER;}
bool Cell::isEmpty() const { return type == Type::EMPTY;}
bool Cell::isTrap() const { return type == Type::TRAP;}
bool Cell::isDamageTrap() const { return type == Type::DAMAGE_TRAP;}
bool Cell::isPassable() const { return type != Type::OBSTACLE && type != Type::TOWER && type != Type::ENEMY && type != Type::DAMAGE_TOWER && type != Type::PLAYER; }

void Cell::setTrap() { type = Type::TRAP; }
void Cell::setDamageTrap() { type = Type::DAMAGE_TRAP; }
void Cell::setObstacle() { type = Type::OBSTACLE; }
void Cell::setPlayer() { type = Type::PLAYER; }
void Cell::setEnemy() { type = Type::ENEMY; }
void Cell::setTower() { type = Type::TOWER; }
void Cell::setDamageTower() { type = Type::DAMAGE_TOWER; }

void Cell::setEmpty() {
    type = Type::EMPTY;
}

void Cell::removeEntity() {
    if (type == Type::PLAYER || type == Type::ENEMY || type == Type::DAMAGE_TOWER) type = Type::EMPTY;
}
