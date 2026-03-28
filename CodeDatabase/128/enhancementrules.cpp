#include "enhancementrules.hpp"
#include "constants.hpp"
#include "spell_defaults.hpp"

SpellParams EnhancementRules::Apply(SpellType type, SpellParams base, int stacks) const {
    if (stacks <= 0) return base;
    switch (type) {
        case SpellType::kDirectDamage:
            base.SetDirectRadius(base.getDirectRadius() + stacks);
            break;
        case SpellType::kAreaDamage:
            base.SetAreaSize(base.getAreaSize() + stacks);
            break;
        case SpellType::kTrapped:
            base.SetTrapDamage(base.getTrapDamage() + stacks *  configs::kTrapDamagePerStack);
            break;
        case SpellType::kSummon:
            base.SetSummonCount(base.getSummonCount() + stacks);
            break;
        case SpellType::kEnhancement:
            break;
        default:
            break;
    }
    return base;
}
