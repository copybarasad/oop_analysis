#include "controllers/controller.h"
#include "controllers/gameView.h"
#include "console/consoleRender.h"
#include "controllers/InputHandler.h"
#include "logger/ConsoleLogger.h"
#include "logger/FileLogger.h"
#include <iostream>

using ConsoleInput = InputHandler<ConsoleInputDevice>;

int main(int argc, char *argv[])
{
    std::shared_ptr<ILogger> logger;

    if (argc > 1)
    {
        std::string mode = argv[1];
        if (mode == "console")
        {
            logger = std::make_shared<ConsoleLogger>();
        }
        else if (mode == "file" && argc > 2)
        {
            logger = std::make_shared<FileLogger>(argv[2]);
        }
        else
        {
            logger = std::make_shared<ConsoleLogger>(); 
        }
    }
    else
    {
        logger = std::make_shared<ConsoleLogger>();
    }

    
    Game game;
    game.setLogger(logger);
    GameController<ConsoleInputDevice> controller("../configs/keymap.json");
    GameView<ConsoleRenderer> view;
    ConsoleRenderer renderer;
    renderer.showStartMenu();

    controller.run(game, view);

    return 0;
}
