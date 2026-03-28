#pragma once
#include "SpellCreator.h"
#include "../../Headers/Realizations/StraightTower.h"
#include "../Containers/DamagerContainer.h"

class StraightTowerCreator
{
    UpdatingController* _updater;
    FieldContainer* _container;
    MortalContainer* _mortalContainer;
    DamagerContainer* _damagerContainer;
    SpellCreator* _spellCreator;
    std::list<StraightTower> _towers;
public:
    StraightTowerCreator(UpdatingController* updater, FieldContainer* container, MortalContainer* mortalContainer,
        DamagerContainer* damagerContainer, SpellCreator* spellCreator);

    void Destroy(StraightTower* tower);

    StraightTower* Create(Cell position, int damage);

    std::list<StraightTower> GetAll();
};