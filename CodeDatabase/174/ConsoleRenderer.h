#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include <string>
#include "Game.h"

class Game;
class GameMenu;

class ConsoleRenderer {
public:
    void renderGame(const Game& game);
    void renderGameEnd(const Game::GameEndData& gameEndData);
    void clearScreen();
};
#endif 