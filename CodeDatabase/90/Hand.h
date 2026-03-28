#ifndef RPG_HAND_H_
#define RPG_HAND_H_

#include "Spell.h"
#include <vector>

class Hand {
private:
  std::vector<SpellPtr> spells;
  size_t capacity;

public:
  explicit Hand(size_t capacity = 5) : capacity(capacity) {}

  size_t size() const { return spells.size(); }
  size_t maxSize() const { return capacity; }
  const std::vector<SpellPtr> &getAll() const { return spells; }
  bool empty() const { return spells.empty(); }

  void add(SpellPtr spell);
  bool addRandom();
  const Spell *getFirstTargetingSpell() const;
  Spell *getFirstTargetingSpell();
  bool consumeFirstTargetingSpell();
  std::vector<std::string> getSpellNames() const;
};

#endif // RPG_HAND_H_