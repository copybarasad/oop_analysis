#include "SpellFactory.h"
#include "DamageSpell.h"
#include "AreaDamageSpell.h"
#include "BuffSpell.h"
#include "SummonSpell.h"
#include "TrapSpell.h"



std::unique_ptr<Spell> SpellFactory::CreateByName(const std::string& name)
{
    if (name == "DamageSpell" || name == "Firebolt") {
        return std::make_unique<DamageSpell>("Firebolt", 5, 3);
    }
    if (name == "AreaDamage") {
        return std::make_unique<AreaDamageSpell>("Explosion", 4, 2);
    }
    if (name == "Buff") {
        return std::make_unique<BuffSpell>("Buff");
    }
    if (name == "Summon") {
        return std::make_unique<SummonSpell>("Summon", 1, 5);
    }
    if (name == "Trap") {
        return std::make_unique<TrapSpell>("Trap", 1);
    }
   
    return Hand::CreateRandomSpell();
}