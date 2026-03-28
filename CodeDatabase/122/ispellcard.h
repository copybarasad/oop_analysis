#ifndef ISPELLCARD_H
#define ISPELLCARD_H

#include "constants.h"

class ISpellConfig;

class ISpellCard
{
public:
    virtual ~ISpellCard() = default;
    virtual void useCard(ISpellConfig& config) = 0;
    virtual void upgrade(int level) = 0;
    virtual void clearUpgrades() = 0;
    virtual bool wasUsed() const = 0;
    virtual int getUpgradeLevel() const = 0;
};

#endif // ISPELLCARD_H
