#include "spell_factory.hpp"

std::shared_ptr<ISpell> SpellFactory::create(char c) {
    switch (c) {
        case 'Z': return std::make_shared<ZapSpell>();
        case 'F': return std::make_shared<FireballSpell>();
        case 'E': return std::make_shared<SpawnEnemySpell>();
        case 'A': return std::make_shared<SummonSpell>();
        case '_': return std::make_shared<TrapSpell>();
        default:  return nullptr;
    }
}