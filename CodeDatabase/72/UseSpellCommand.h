//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_USESPELLCOMMAND_H
#define GAME_TERM_USESPELLCOMMAND_H
#include "Command.h"


class UseSpellCommand: public Command{

public:

    CommandType getType() const override;

    CommandResult execute(Game &game) override;
};


#endif //GAME_TERM_USESPELLCOMMAND_H