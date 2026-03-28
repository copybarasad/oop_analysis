#include "spell_factory.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "enhancement_spell.h"
#include <cstdlib>

std::unique_ptr<Spell> CreateRandomSpell() {
    int t = rand() % 5;
    switch (t) {
        case 0: return std::make_unique<DirectDamageSpell>();
        case 1: return std::make_unique<AreaDamageSpell>();
        case 2: return std::make_unique<TrapSpell>();
        case 3: return std::make_unique<SummonSpell>();
        default: return std::make_unique<EnhancementSpell>();
    }
}