#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Spell.h"
#include "types/AOEDamageSpell.h"
#include "types/DamageSpell.h"
#include "types/SummonSpell.h"
#include "types/TrapSpell.h"
#include "types/UpgradeSpell.h"

class SpellRegistry {
public:
    static SpellRegistry& instance() {
        static SpellRegistry inst;

        return inst;
    }

    void register_spell(Spell* spell);

    Spell* get(const std::string& id) const;

    std::vector<std::string> ids() const;

    void cleanup();

private:
    SpellRegistry() {
        register_spell(new AOEDamageSpell());
        register_spell(new DamageSpell());
        register_spell(new SummonSpell());
        register_spell(new TrapSpell());
        register_spell(new UpgradeSpell());
    }
    ~SpellRegistry() = default;

    SpellRegistry(const SpellRegistry&) = delete;
    SpellRegistry& operator=(const SpellRegistry&) = delete;

    std::unordered_map<std::string, Spell*> registry_;
};
