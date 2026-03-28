#pragma once
#include "game.hpp"

class GameController {
private:
    Game game_;

public:
    GameController() = default;
    void Run();
    void ShowUpgradeMenu();
};