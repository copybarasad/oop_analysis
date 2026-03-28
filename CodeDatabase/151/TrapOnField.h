#pragma once
#ifndef TRAP_ON_FIELD_H
#define TRAP_ON_FIELD_H

#include "Position.h"

class TrapOnField {
private:
    Position position;
    int damage;
    bool active;

public:
    TrapOnField(int x, int y, int damage);

    bool checkTrigger(const Position& target) const;
    int getDamage() const;
    Position getPosition() const;
    bool isActive() const;
    void deactivate();
};

#endif