#ifndef CONTINUE_GAME_COMMAND_H
#define CONTINUE_GAME_COMMAND_H

#include "command_interface.h"
#include <string>

class GameController;

class ContinueGameCommand : public ICommand {
public:
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override;
    std::string getErrorMessage() const override;
};

#endif