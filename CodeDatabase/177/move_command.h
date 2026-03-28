#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "command_interface.h"
#include "game_enums.h"

class MoveCommand : public ICommand {
private:
    Direction direction;
    
public:
    explicit MoveCommand(Direction dir);
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::Move; }
};

#endif