#pragma once
#include "command.h"
#include "managment/game.h"
#include "controllers/IGameView.h"
#include "console/consoleInputDevice.h"
#include "InputHandler.h"

template <typename InputDevice>
class GameController
{
    InputHandler<InputDevice> inputHandler;

public:
    template <typename... Args>
    GameController(Args &&...args) : inputHandler(std::forward<Args>(args)...) {}

    template <typename GameType, typename GameViewType>
    void run(GameType &game, GameViewType &view)
    {
        char startOption;
        std::cin >> startOption;
        if (startOption == 'L')
        {
            game.loadGame(view, inputHandler);
        }

        while (game.isRunning())
        {
            view.update(game);

            Command cmd = inputHandler.getCommand();
            game.handleCommand(cmd, view, inputHandler);

            if (game.levelUp())
            {
                Command upCmd = {CommandType::LevelUp};
                game.update(upCmd, view, inputHandler);
            }
            else
            {
                Command none{CommandType::None};
                game.update(none, view, inputHandler);
            }
        }

        view.showGameOver(game);
    }
};
