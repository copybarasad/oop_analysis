//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_QUITCOMMAND_H
#define GAME_TERM_QUITCOMMAND_H
#include "Command.h"


class QuitCommand: public Command{

public:
    CommandType getType() const override;

    CommandResult execute(Game &game) override;

};


#endif //GAME_TERM_QUITCOMMAND_H