#pragma once
#include <list>
#include "../Containers/DamagerContainer.h"
#include "../Controllers/MovableController.h"
#include "../Containers/MortalContainer.h"
#include "../Realizations/Enemy.h"

class EnemyCreator
{
    MovableController* _controller;
    FieldContainer* _container;
    MortalContainer* _mortalContainer;
    DamagerContainer* _damagerContainer;
    std::list<Enemy> _enemies;
public:
    EnemyCreator(MovableController* controller, FieldContainer* container, MortalContainer* mortalContainer, 
        DamagerContainer* damagerContainer);

    void Destroy(Enemy* enemy);

    Enemy* Create(Cell position, int speed, int maxHealth, int damage);

    std::list<Enemy> GetAll();
};