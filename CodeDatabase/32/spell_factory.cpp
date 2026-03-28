#include "spell_factory.hpp"
#include "direct_damage_spell.hpp"
#include "area_damage_spell.hpp"
#include "trap.hpp"
#include "summon_spell.hpp"
#include "enhancementspell.hpp"

#include <cstdlib>
#include <ctime>

SpellFactory::SpellFactory() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

std::unique_ptr<Spell> SpellFactory::GenerateRandomSpell() const {
    static int counter = 0;
    int choice = counter % 5;
    ++counter;

    switch (choice) {
        case 0: return std::make_unique<DirectDamageSpell>();
        case 1: return std::make_unique<AreaDamageSpell>();
        case 2: return std::make_unique<TrapSpell>();
        case 3: return std::make_unique<SummonSpell>();
        case 4: return std::make_unique<EnhancementSpell>();
        default: return std::make_unique<DirectDamageSpell>();
    }
}

std::unique_ptr<Spell> SpellFactory::CreateByType(int typeId) {
    switch (typeId) {
        case DIRECT_DAMAGE:  return std::make_unique<DirectDamageSpell>();
        case AREA_DAMAGE:    return std::make_unique<AreaDamageSpell>();
        case TRAP:           return std::make_unique<TrapSpell>();
        case SUMMON:         return std::make_unique<SummonSpell>();
        case ENHANCEMENT:    return std::make_unique<EnhancementSpell>();
        default:             return std::make_unique<DirectDamageSpell>();
    }
}

int SpellFactory::DetectTypeId(const Spell& spell) {
    if (dynamic_cast<const DirectDamageSpell*>(&spell))   return DIRECT_DAMAGE;
    if (dynamic_cast<const AreaDamageSpell*>(&spell))     return AREA_DAMAGE;
    if (dynamic_cast<const TrapSpell*>(&spell))           return TRAP;
    if (dynamic_cast<const SummonSpell*>(&spell))         return SUMMON;
    if (dynamic_cast<const EnhancementSpell*>(&spell))    return ENHANCEMENT;
    return DIRECT_DAMAGE;
}

void SpellFactory::ApplyUpgradeLevel(Spell& spell, int level) {
    if (level <= 0) return;

    for (int i = 0; i < level; ++i) {
        if (auto* d = dynamic_cast<DirectDamageSpell*>(&spell))        d->Upgrade();
        else if (auto* a = dynamic_cast<AreaDamageSpell*>(&spell))     a->Upgrade();
        else if (auto* t = dynamic_cast<TrapSpell*>(&spell))           t->Upgrade();
        else if (auto* s = dynamic_cast<SummonSpell*>(&spell))         s->Upgrade();
        else if (auto* e = dynamic_cast<EnhancementSpell*>(&spell))    e->Upgrade();
        else break;
    }
}