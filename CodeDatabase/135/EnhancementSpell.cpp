#include "EnhancementSpell.h"
#include "SpellContext.h"
#include "SpellEnhancer.h"

EnhancementSpell::EnhancementSpell() {}

bool EnhancementSpell::cast(SpellContext& context) {
	context.getEnhancer().addEnhancement();
	return true;
}
