#pragma once
#include "CompositionRootN1.h"
#include "Hand.h"
#include "../Controllers/UpdatingController.h"
#include "../Facades/StraightTowerCreator.h"
#include "../Facades/SpellCreator.h"

struct CompositionRootN2
{
    CompositionRootN1& _root1;
    UpdatingContainer _updatingContainer;
    UpdatingController _updater;
    Hand _playerHand;
    SpellCreator _spellCreator;
    StraightTowerCreator _towerCreator;

    explicit CompositionRootN2(CompositionRootN1& root1);
};