#include "node.h"
#include "player.h"
#include "enemy.h"

Node::Node() : type(Type::EMPTY), trapDamage(0) {};

Type Node::getType() const { return type; }

bool Node::hasPlayer() const { return type == Type::PLAYER;}
bool Node::hasEnemy() const { return type == Type::ENEMY;}
bool Node::hasObstacle() const { return type == Type::OBSTACLE;};
bool Node::isEmpty() const { return type == Type::EMPTY;}
bool Node::isPassable() const { return type != Type::OBSTACLE && type != Type::TOWER;}


void Node::setObstacle() { 
    type = Type::OBSTACLE;
    trapDamage = 0;
}

void Node::setPlayer() { 
    type = Type::PLAYER;
    trapDamage = 0;
}
void Node::setEnemy() { 
    type = Type::ENEMY;
    trapDamage = 0;
}
void Node::setEmpty() {
    type = Type::EMPTY;
    trapDamage = 0;
}

void Node::removeEntity() {
    if (type == Type::PLAYER || type == Type::ENEMY) type = Type::EMPTY;
}

void Node::removeObstacle() {
    if (type == Type::OBSTACLE) type = Type::EMPTY;
}
bool Node::hasTrap() const { return type == Type::TRAP; }
void Node::setTrap(int damage) {
    type = Type::TRAP;
    trapDamage = damage;
}
void Node::removeTrap() {
    if (type == Type::TRAP) {
        type = Type::EMPTY;
        trapDamage = 0;
    }
}
int Node::triggerTrap() {
    if (type == Type::TRAP) {
        int damage = trapDamage;
        setEmpty();
        return damage;
    }
    return 0;
}
bool Node::hasTower() const { return type == Type::TOWER; }

void Node::setTower() { 
    type = Type::TOWER;
    trapDamage = 0;
}
void Node::removeTower() {
    if (type == Type::TOWER) {
        type = Type::EMPTY;
        trapDamage = 0;
    }
}
