#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "command.hpp"
#include <memory>
#include <iostream>

enum class GameMode {
    MAIN_MENU,
    PAUSE_MENU,
    SAVE_MENU,
    IN_GAME
};

class TerminalInputHandler {
public:
    std::unique_ptr<Command> getNextCommand(GameMode mode);

private:
    std::unique_ptr<MenuCommand> readMainMenuCommand();
    std::unique_ptr<MenuCommand> readPauseMenuCommand();
    std::unique_ptr<MenuCommand> readSaveMenuCommand();
    std::unique_ptr<GameCommand> readGameCommand();
};

#endif