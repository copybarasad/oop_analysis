#ifndef COMMANDS_QUITE_GAME_COMMAND_H
#define COMMANDS_QUITE_GAME_COMMAND_H

#include "ICommand.h"

class QuitGameCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif