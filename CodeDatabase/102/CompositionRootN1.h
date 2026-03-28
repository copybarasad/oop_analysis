#pragma once
#include <vector>
#include "../Controllers/MovableController.h"
#include "DamagerContainer.h"
#include "../Facades/PlayerCreator.h"
#include "../Facades/EnemyCreator.h"

struct CompositionRootN1
{
    FieldContainer _container;
    MovableContainer _movableContainer;
    MortalContainer _mortalContainer;
    DamagerContainer _damagerContainer;
    MovableController _movableController;
    PlayerCreator _playerCreator;
    EnemyCreator _enemyCreator;

    CompositionRootN1(int width, int height);

    CompositionRootN1(int width, int height, std::vector<Cell>& walls);
};