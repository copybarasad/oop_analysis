#pragma once
#include <algorithm>
#include <random>

#include "ISpell.h"

class Hand {
 public:
  explicit Hand(size_t size = 3);
  bool AddSpell(std::unique_ptr<ISpell> spell);
  std::unique_ptr<ISpell> TakeSpell(size_t index);
  std::vector<std::unique_ptr<ISpell>>& GetSpells();
  size_t Size() const;
  void SetMaxSize(size_t size);
  bool IsFull() const;
  void DiscardRandomHalf();
  void IncreaseCapacity(size_t amount) { capacity += amount; }
  size_t MaxSize() const;

 private:
  size_t capacity;
  std::vector<std::unique_ptr<ISpell>> spells;
};
