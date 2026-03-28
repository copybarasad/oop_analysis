#include "spellfactory.hpp"
#include "directdamage.hpp"
#include "areadamage.hpp"
#include "trapped.hpp"
#include "summon.hpp"
#include "enhancementspell.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>

std::unique_ptr<Spell> SpellFactory::CreateSpell(SpellType type) const {
    switch (type) {
        case SpellType::kDirectDamage: return std::make_unique<DirectDamageSpell>();
        case SpellType::kAreaDamage:   return std::make_unique<AreaDamageSpell>();
        case SpellType::kTrapped:      return std::make_unique<Trapped>();
        case SpellType::kSummon:       return std::make_unique<Summon>();
        case SpellType::kEnhancement:  return std::make_unique<EnhancementSpell>();
    }
    return nullptr;
}

std::unique_ptr<Spell> SpellFactory::CreateRandomSpell() const {
    std::vector<SpellType> pool = {
        SpellType::kDirectDamage,
        SpellType::kAreaDamage,
        SpellType::kTrapped,
        SpellType::kSummon,
        SpellType::kEnhancement
    };
    srand(static_cast<unsigned>(time(nullptr)));
    int i = rand() % pool.size();
    return CreateSpell(pool[i]);
}


SpellType SpellFactory::FromString(const std::string& name) {
    if (name == "kDirectDamage") return SpellType::kDirectDamage;
    if (name == "kAreaDamage")   return SpellType::kAreaDamage;
    if (name == "kSummon")       return SpellType::kSummon;
    if (name == "kTrapped")      return SpellType::kTrapped;
    if (name == "kEnhancement")  return SpellType::kEnhancement;

    throw std::runtime_error("Unknown spell name: " + name);
}