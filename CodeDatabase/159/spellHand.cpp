#include "spellHand.h"
#include "directDamageSpell.h"
#include "areaDamageSpell.h"
#include "trapSpell.h"
#include "summonAllySpell.h"
#include "enhancementSpell.h"
#include <stdexcept>
#include <random>
#include <algorithm>

SpellHand::SpellHand(int maxSize): maxSize(maxSize) {}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
  if (spells.size()<maxSize) {
    spells.push_back(std::move(spell));
    return true;
  }
  return false;
}

bool SpellHand::addRandomSpell() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 4);
  int randomIndex = dis(gen);

  std::unique_ptr<Spell> spell;
  if (randomIndex == 0) {
    spell = std::make_unique<DirectDamageSpell>();
  } else if (randomIndex == 1) {
    spell = std::make_unique<AreaDamageSpell>();
  } else if (randomIndex == 2) {
    spell = std::make_unique<TrapSpell>();
  } else if (randomIndex == 3) {
    spell = std::make_unique<SummonAllySpell>();
  } else {
    spell = std::make_unique<EnhancementSpell>();
  }
  return addSpell(std::move(spell));
}

const std::vector<std::unique_ptr<Spell>>& SpellHand::getSpells() const {
  return spells;
}

const Spell& SpellHand::selectSpell(int index) const {
  if (index < 0 || index >= static_cast<int>(spells.size())) {
    throw std::out_of_range("Invalid spell index");
  }
  return *spells[index];
}

void SpellHand::removeSpell(int index) {
  if (index < 0 || index >= static_cast<int>(spells.size())) {
    throw std::out_of_range("Invalid spell index");
  }
  spells.erase(spells.begin()+index);
}

size_t SpellHand::getSize() const {return spells.size();}

size_t SpellHand::getMaxSize() const {return maxSize;}

std::vector<std::string> SpellHand:: getAvailableSpellDescriptions() const {
  return {"Direct Damage Spell", "Area Damage Spell", "Trap Spell", "Summon Ally Spell", "Enhancement Spell"};
}

std::unique_ptr<Spell> SpellHand::createSpell(SpellType type) const {
  switch (type) {
    case SpellType::DirectDamageSpell: return std::make_unique<DirectDamageSpell>();
    case SpellType::AreaDamageSpell: return std::make_unique<AreaDamageSpell>();
    case SpellType::TrapSpell: return std::make_unique<TrapSpell>();
    case SpellType::SummonAllySpell: return std::make_unique<SummonAllySpell>();
    case SpellType::EnhancementSpell: return std::make_unique<EnhancementSpell>();
    default: return nullptr;
  }
}

void SpellHand::clear() { spells.clear(); }