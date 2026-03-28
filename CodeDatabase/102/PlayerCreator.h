#pragma once
#include <list>
#include "../Containers/DamagerContainer.h"
#include "../Controllers/MovableController.h"
#include "../Containers/MortalContainer.h"
#include "../Realizations/Player.h"

class PlayerCreator
{
    MovableController* _controller;
    FieldContainer* _container;
    MortalContainer* _mortalContainer;
    DamagerContainer* _damagerContainer;
    std::list<Player> _players;
public:
    PlayerCreator(MovableController* controller, FieldContainer* container, MortalContainer* mortalContainer, 
        DamagerContainer* damagerContainer);

    void Destroy(Player* player);

    Player* Create(Cell position, int speed, int maxHealth, int nearDamage, int farDamage);

    std::list<Player> GetAll();
};