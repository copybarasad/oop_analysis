#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "command.h"

class Game;

template <typename InputReader, typename View>
class GameController
{
public:
    GameController(Game& gameRef);

    void run();

private:
    void showMainMenu();
    void mainLoop();
    void showPauseMenu();
    int getValidInput(int minChoice, int maxChoice);

private:
    Game& game;
    InputReader reader;
    View view;
    bool running = true;
    bool exitGame = false;
    bool returnToMenu = false;
};

#include "game_controller_impl.hpp"

#endif
