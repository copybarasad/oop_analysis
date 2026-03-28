#ifndef ISPELLFACTORY_H
#define ISPELLFACTORY_H

#include "ISpell.h"
#include <memory>

class ISpellFactory {
public:
    virtual ~ISpellFactory() = default;
    virtual std::unique_ptr<ISpell> createSpell(SpellType type) = 0;
};

#endif



