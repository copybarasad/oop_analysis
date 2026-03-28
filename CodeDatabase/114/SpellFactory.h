#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H

#include "ISpellFactory.h"

class SpellFactory : public ISpellFactory {
public:
    std::unique_ptr<ISpell> createSpell(SpellType type) override;
};

#endif



