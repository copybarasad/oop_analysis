#pragma once
#include "Field.h"
#include "Player.h"
#include "Game.h"
#include <string>
#include <vector>

class ConsoleRenderer: public IRenderer {
public:
    ConsoleRenderer();

    void render(const Game& game) override;

    static void clearConsole();
};