#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H

#include <memory>
#include "ISpell.h"

class SpellFactory {
public:
    static std::unique_ptr<ISpell> createRandomSpell();
};

#endif