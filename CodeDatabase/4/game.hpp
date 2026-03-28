
#pragma once
#include "hero.hpp"
#include "field.hpp"
#include "entitymanager.hpp"

class Game {
    int level = 1;
    Hero player;
    EntityManager roster;
    Field field;
public:
    Game();
    void loop();
};