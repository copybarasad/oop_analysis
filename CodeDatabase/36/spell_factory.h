#ifndef SPELL_FACTORY_H
#define SPELL_FACTORY_H

#include "spell.h"
#include <memory>

std::unique_ptr<Spell> CreateRandomSpell();

#endif