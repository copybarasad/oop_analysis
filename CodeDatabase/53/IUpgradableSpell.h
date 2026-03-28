#ifndef IUPGRADABLESPELL_H
#define IUPGRADABLESPELL_H

#include "ISpell.h"

class IUpgradableSpell : public virtual ISpell {
public:
    virtual ~IUpgradableSpell() = default;
    virtual void upgrade() = 0;
    virtual bool canUpgrade() = 0;
    virtual void resetLevel() = 0;
};

#endif