//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_MOVECOMMAND_H
#define GAME_TERM_MOVECOMMAND_H
#include "Command.h"
#include "../Direction.h"


class MoveCommand: public Command{

    Direction direction;

public:

    explicit MoveCommand(const Direction & dir);

    CommandType getType() const override;


     CommandResult execute(Game &game) override;
};


#endif //GAME_TERM_MOVECOMMAND_H