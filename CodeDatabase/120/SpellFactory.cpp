#include "SpellFactory.h"

std::unique_ptr<ISpell> SpellFactory::CreateSpell(const std::string& name) {
  if (name == "Fireball") return std::make_unique<DamageSpell>(10, 3);
  if (name == "Meteor") return std::make_unique<AoESpell>(20, 2);
  if (name == "Trap") return std::make_unique<TrapSpell>(5);
  if (name == "Ally") return std::make_unique<SummonSpell>(1);
  if (name == "Buff") return std::make_unique<BuffSpell>();
  return nullptr;
}
