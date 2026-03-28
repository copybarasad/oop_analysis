#include "SpellFactory.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"

std::unique_ptr<Spell> SpellFactory::createMagicMissile() {
    return std::make_unique<DirectDamageSpell>("Magic Missile", 30, 5);
}

std::unique_ptr<Spell> SpellFactory::createFireball() {
    return std::make_unique<AreaDamageSpell>("Fireball", 35, 4, 2);
}

std::unique_ptr<Spell> SpellFactory::createTrap() {
    return std::make_unique<TrapSpell>("Trap", 45, 1);
}
