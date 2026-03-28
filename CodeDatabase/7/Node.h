#pragma once
#include "Type.h"

class Node {
private:
    Type type;
    int trapDamage;

public:
    Node();
    Type getType() const;
    void setType(Type newType);
    bool isPassable() const;
    void setTrap(int damage);
    int triggerTrap();
};