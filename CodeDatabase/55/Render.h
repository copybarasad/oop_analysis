#pragma once
#include <string>
#include "Game.h"


class Game;

class ConsoleRenderer {
public:
    void renderMenu();
    void renderControls();
    void renderStatistics(Game& game);
    void renderField(Game& game);
    void clearScreen();
};