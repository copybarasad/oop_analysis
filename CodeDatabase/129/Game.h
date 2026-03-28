#pragma once
#include "../game/utils/Values.h"
#include "GManager.h"

class Game{
private:
    GManager gmanager;
public:
    Game();

    void process();

    ~Game();
};
