//
// Created by Artem on 30.10.2025.
//

#ifndef LABAOOP2_SPELLFACTORY_H
#define LABAOOP2_SPELLFACTORY_H
#include"../Entities/Creatures/Attacks/Spels/Spell.h"
#include "AllySpawner.h"

class SpellFactory {
public:
    explicit SpellFactory(AllySpawner &allySpawner) : allySpawner(allySpawner) {}

    virtual std::unique_ptr<Spell> createSpell(SpellType type) final;
    virtual std::unique_ptr<Spell> createRandomSpell() final;
private:
    std::vector<SpellType> types = {SpellType::DirectDamageSpell, SpellType::CreateTrapSpell, SpellType::SummoningSpell, SpellType::AreaDamageSpell, SpellType::BuffNextUsedSpell};
    AllySpawner& allySpawner;
};


#endif //LABAOOP2_SPELLFACTORY_H
