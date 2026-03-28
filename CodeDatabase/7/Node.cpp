#include "Node.h"

Node::Node() : type(Type::EMPTY), trapDamage(0) {}

Type Node::getType() const {
    return type;
}

void Node::setType(Type newType) {
    type = newType;
    if (newType != Type::TRAP) {
        trapDamage = 0;
    }
}

bool Node::isPassable() const {
    return type != Type::OBSTACLE;
}

void Node::setTrap(int damage) {
    type = Type::TRAP;
    trapDamage = damage;
}

int Node::triggerTrap() {
    if (type == Type::TRAP) {
        int damage = trapDamage;
        setType(Type::EMPTY);
        return damage;
    }
    return 0;
}