#ifndef SAVE_GAME_COMMAND_H
#define SAVE_GAME_COMMAND_H

#include "command_interface.h"

class SaveGameCommand : public ICommand {
public:
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::SaveGame; }
};

#endif