#pragma once

#include <iostream>
#include "GameController.h"

class Game; 
// Класс, отвечающий за отрисовку состояния игры
class GameRenderer {
public:
    GameRenderer() = default;

    void render(const Game& game, const GameController& controller) const;
};
// forward declaration