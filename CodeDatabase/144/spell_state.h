#ifndef SPELL_STATE_H
#define SPELL_STATE_H

#include "spell.h"
#include <memory>

struct SpellState {
	SpellType type;
	int enhancementLevel;
};

class SpellSerializer {
public:
	SpellState serialize(const Spell& spell) const;
	std::unique_ptr<Spell> deserialize(const SpellState& state) const;
};

#endif

