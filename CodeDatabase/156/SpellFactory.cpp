//
// Created by Artem on 30.10.2025.
//

#include "SpellFactory.h"
#include "../Entities/Creatures/Attacks/Spels/DirectDamageSpell.h"
#include "../Entities/Creatures/Attacks/Spels/AreaDamageSpell.h"
#include "../Entities/Creatures/Attacks/Spels/SummoningSpell.h"
#include "../Entities/Creatures/Attacks/Spels/BuffSpellSpell.h"
#include "../Entities/Creatures/Attacks/Spels/CreateTrapSpell.h"

std::unique_ptr<Spell> SpellFactory::createSpell(SpellType type) {
    switch (type) {
        case SpellType::DirectDamageSpell:
        {
            auto* spell = new DirectDamageSpell();
            return std::make_unique<DirectDamageSpell>(*spell);
        }
        case SpellType::AreaDamageSpell:
        {
            auto* spell = new AreaDamageSpell();
            return std::make_unique<AreaDamageSpell>(*spell);
        }
        case SpellType::SummoningSpell:
        {
            auto* spell = new SummoningSpell(this->allySpawner);
            return std::make_unique<SummoningSpell>(*spell);
        }
        case SpellType::BuffNextUsedSpell:
        {
            auto* spell = new BuffSpellSpell();
            return std::make_unique<BuffSpellSpell>(*spell);
        }
        case SpellType::CreateTrapSpell:
        {
            auto* spell = new CreateTrapSpell();
            return std::make_unique<CreateTrapSpell>(*spell);
        }
    }
    throw UnexpectedBehaviorException("ERROR!!!!!");
}

std::unique_ptr<Spell> SpellFactory::createRandomSpell() {
    return createSpell(this->types.at(rand() % this->types.size()));
}
