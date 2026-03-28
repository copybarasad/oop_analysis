#ifndef QUIT_GAME_COMMAND_H
#define QUIT_GAME_COMMAND_H

#include "command_interface.h"

class QuitGameCommand : public ICommand {
public:
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::QuitGame; }
};

#endif