#include "Hand.h"

Hand::Hand(size_t size) : capacity(size) {}

bool Hand::AddSpell(std::unique_ptr<ISpell> spell) {
  if (spells.size() >= capacity) return false;
  spells.push_back(std::move(spell));
  return true;
}

std::unique_ptr<ISpell> Hand::TakeSpell(size_t index) {
  if (index > spells.size()) return nullptr;
  auto ptr = std::move(spells[index]);
  spells.erase(spells.begin() + index);
  return ptr;
}
bool Hand::IsFull() const { return spells.size() >= capacity; }
void Hand::SetMaxSize(size_t size) { capacity = size; }
std::vector<std::unique_ptr<ISpell>>& Hand::GetSpells() { return spells; }
size_t Hand::Size() const { return spells.size(); }
size_t Hand::MaxSize() const { return capacity; }

void Hand::DiscardRandomHalf() {
  if (spells.empty()) return;
  size_t toRemove = spells.size() / 2;
  if (toRemove == 0) return;

  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<size_t> idx(spells.size());
  for (size_t i = 0; i < idx.size(); ++i) idx[i] = i;
  std::shuffle(idx.begin(), idx.end(), gen);

  std::vector<std::unique_ptr<ISpell>> newSpells;
  std::vector<bool> removeMask(spells.size(), false);
  for (size_t i = 0; i < toRemove; ++i) removeMask[idx[i]] = true;
  for (size_t i = 0; i < spells.size(); ++i) {
    if (!removeMask[i]) newSpells.push_back(std::move(spells[i]));
  }
  spells = std::move(newSpells);
}