#include "SpellStore.hpp"

SpellStore::SpellStore (std::vector<std::string> spells) :
    store(spells) {}

size_t SpellStore::getSpellNumber () const { return store.size(); }

void SpellStore::removeSpell (size_t index) {
    store.erase(store.begin() + index);
}

std::string SpellStore::getSpell (size_t index) const {
    return store[index];
}