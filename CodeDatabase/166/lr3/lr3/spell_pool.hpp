#ifndef SPELL_POOL_HPP_INCLUDED
#define SPELL_POOL_HPP_INCLUDED

#include <memory>
#include "spell.hpp"

// фабрика одной случайной карты из пула
std::unique_ptr<Spell> makeRandomSpell();

#endif
