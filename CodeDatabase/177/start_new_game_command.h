#ifndef START_NEW_GAME_COMMAND_H
#define START_NEW_GAME_COMMAND_H

#include "command_interface.h"

class StartNewGameCommand : public ICommand {
public:
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::StartNewGame; }
};

#endif