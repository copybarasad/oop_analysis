#pragma once
#include "GameLogic.h"


class ConsoleRenderer {
public:

    void Render(GameWorld& world, GameLogic& logic);

private:
    void ClearConsole();
    void Print(GameWorld& world, const GameLogic& logic);
};
