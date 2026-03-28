#ifndef SPELL_FACTORY_H
#define SPELL_FACTORY_H

#include <memory>
#include "Spell.h"

class SpellFactory {
public:
	static std::unique_ptr<Spell> create(SpellId id);
	static std::unique_ptr<Spell> createUpgraded(SpellId id);
	static std::unique_ptr<Spell> createRandomBasic();

private:
	static SpellId randomId();
};

#endif

