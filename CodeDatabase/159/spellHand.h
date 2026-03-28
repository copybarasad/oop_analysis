#ifndef SPELLHAND_H
#define SPELLHAND_H

#include <memory>
#include <vector>
#include <string>
#include "spell.h"

class SpellHand {
public:
  explicit SpellHand(int maxSize);
  bool addSpell(std::unique_ptr<Spell> spell);
  bool addRandomSpell();
  const std::vector<std::unique_ptr<Spell>>& getSpells() const;
  const Spell& selectSpell(int index) const;
  void removeSpell(int index);
  void clear();
  
  size_t getSize() const;
  size_t getMaxSize() const;

  std::vector<std::string> getAvailableSpellDescriptions() const;
  std::unique_ptr<Spell> createSpell(SpellType type) const;

private:
  std::vector<std::unique_ptr<Spell>> spells;
  size_t maxSize;
};

#endif