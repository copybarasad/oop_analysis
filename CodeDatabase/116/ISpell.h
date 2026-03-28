#pragma once
#include <iostream>
#include <vector>

class ISpellContext; 

class ISpell{
public:
    virtual ~ISpell() = default;
    virtual void cast(ISpellContext& context) = 0;
    virtual void upgradeCard(int dlvl = 1) = 0;
    virtual void resetUpgrades() = 0;
    virtual bool wasUsed() = 0;
    virtual int getLVL() = 0;
    virtual std::string getName() = 0;
};
