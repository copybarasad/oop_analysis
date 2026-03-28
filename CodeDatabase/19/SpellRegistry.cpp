#include "SpellRegistry.h"

void SpellRegistry::register_spell(Spell *spell) {
    if (spell == nullptr) return;
    if (!registry_.contains(spell->id())) {
        registry_[spell->id()] = spell;
    }
}

Spell * SpellRegistry::get(const std::string &id) const {
    const auto it = registry_.find(id);
    return (it != registry_.end()) ? it->second : nullptr;
}

std::vector<std::string> SpellRegistry::ids() const {
    std::vector<std::string> ids;
    ids.reserve(registry_.size());
    for (auto& [id, _] : registry_)
        ids.push_back(id);
    return ids;
}

void SpellRegistry::cleanup() {
    for (auto& [_, ptr] : registry_) {
        delete ptr;
    }
    registry_.clear();
}
