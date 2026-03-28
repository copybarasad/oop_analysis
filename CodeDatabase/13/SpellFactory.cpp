#include "SpellFactory.h"

Spell *SpellFactory::create(const std::string &spellName) {
    auto it = this->spells_.find(spellName);
    if (it != this->spells_.end()) {
        Spell *spell = it->second();
        return spell;
    }
    return nullptr;
}
