#ifndef GAME_SPELLFACTORY_H
#define GAME_SPELLFACTORY_H

#include "Spell.h"
#include <unordered_map>
#include <functional>

#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include  "TrapSpell.h"
#include "AllySpell.h"
#include "ImprovingSpell.h"

class SpellFactory {
private:
    std::unordered_map<std::string, std::function<Spell*()> > spells_ = {
        {"Direct damage spell", []() -> Spell * { return new DirectDamageSpell(); }},
        {"Area damage spell", []() -> Spell * { return new AreaDamageSpell(); }},
        {"Trap spell", []() -> Spell * { return new TrapSpell(); }},
        {"Ally spell", []() -> Spell * { return new AllySpell(); }},
        {"Improving spell", []() -> Spell * { return new ImprovingSpell(); }}
    };

public:
    SpellFactory() = default;

    Spell *create(const std::string &spellName);
};


#endif
