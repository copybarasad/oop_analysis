//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_SHOOTCOMMAND_H
#define GAME_TERM_SHOOTCOMMAND_H
#include "Command.h"
#include "../Direction.h"

class ShootCommand: public Command{

    Direction direction;

public:

    explicit ShootCommand(const Direction & dir);

    CommandType getType() const override;

    CommandResult execute(Game &game) override;

};


#endif //GAME_TERM_SHOOTCOMMAND_H