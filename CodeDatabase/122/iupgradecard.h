#ifndef IUPGRADECARD_H
#define IUPGRADECARD_H

#include "upgradespellconfig.h"
#include "ispellcard.h"

class IUpgradeCard : public ISpellCard
{
public:
    virtual ~IUpgradeCard() = default;
    virtual void apply(UpgradeSpellConfig& config) = 0;
};

#endif // IUPGRADECARD_H
