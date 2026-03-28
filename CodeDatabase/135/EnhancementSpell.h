#ifndef ENHANCEMENT_SPELL_H
#define ENHANCEMENT_SPELL_H

#include "ISpell.h"

class EnhancementSpell : public ISpell {
public:
	EnhancementSpell();
	bool cast(SpellContext& context) override;
};

#endif
