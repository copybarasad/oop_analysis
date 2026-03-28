#ifndef COMMANDS_SAVE_GAME_COMMAND_H
#define COMMANDS_SAVE_GAME_COMMAND_H

#include "ICommand.h"

class SaveGameCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif