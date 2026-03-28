#ifndef IATTACKER_H
#define IATTACKER_H

#include "AttackType.h"

class IAttacker {
public:
    virtual ~IAttacker() = default;
    virtual int getDamage() const = 0;
    virtual AttackType getAttackType() const = 0;
};

#endif // IATTACKER_H