//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_SAVECOMMAND_H
#define GAME_TERM_SAVECOMMAND_H
#include "Command.h"


class SaveCommand: public Command{

    bool mode;

public:

    explicit SaveCommand(const bool & mode);

    CommandType getType() const override;

    CommandResult execute(Game &game) override;
};


#endif //GAME_TERM_SAVECOMMAND_H