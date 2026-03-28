//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_UNKNOWNCOMMAND_H
#define GAME_TERM_UNKNOWNCOMMAND_H
#include "Command.h"


class UnknownCommand: public Command{

    char inputChar;

public:

    explicit UnknownCommand(const char & ch);

    CommandType getType() const override;

    CommandResult execute(Game &game) override;

};


#endif //GAME_TERM_UNKNOWNCOMMAND_H