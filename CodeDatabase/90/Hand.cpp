#include "Hand.h"
#include "Random.h"
#include "SpellDirectDamage.h"
#include "SpellSquareDamage.h"

void Hand::add(SpellPtr spell) {
  if (spells.size() < capacity && spell) {
    spells.push_back(std::move(spell));
  }
}

bool Hand::addRandom() {
  if (spells.size() >= capacity) {
    return false;
  }
  if (Random::getInt(0, 1) == 0) {
    add(std::make_unique<SpellDirectDamage>(3, 20));
  } else {
    add(std::make_unique<SpellSquareDamage>(3, 12));
  }
  return true;
}

const Spell *Hand::getFirstTargetingSpell() const {
  for (const auto &s : spells) {
    if (s && s->requiresTarget()) {
      return s.get();
    }
  }
  return nullptr;
}

Spell *Hand::getFirstTargetingSpell() {
  for (auto &s : spells) {
    if (s && s->requiresTarget()) {
      return s.get();
    }
  }
  return nullptr;
}

bool Hand::consumeFirstTargetingSpell() {
  for (size_t i = 0; i < spells.size(); ++i) {
    if (spells[i] && spells[i]->requiresTarget()) {
      spells.erase(spells.begin() + static_cast<long>(i));
      return true;
    }
  }
  return false;
}

std::vector<std::string> Hand::getSpellNames() const {
  std::vector<std::string> names;
  names.reserve(spells.size());
  for (const auto &s : spells) {
    if (s)
      names.push_back(s->name());
  }
  return names;
}
