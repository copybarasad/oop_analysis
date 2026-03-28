#ifndef SPELL_CREATION_H
#define SPELL_CREATION_H

#include "spell.h"
#include "single_target.h"
#include "area.h"
#include "traps.h"
#include <memory>

std::unique_ptr<ISpell> createRandomSpell();

#endif