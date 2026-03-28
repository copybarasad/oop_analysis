//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_SWITCHFIGHTMODECOMMAND_H
#define GAME_TERM_SWITCHFIGHTMODECOMMAND_H
#include "Command.h"
#include "../Game.h"


class SwitchFightModeCommand: public Command{

public:
    CommandType getType() const override;

    CommandResult execute(Game &game) override;
};


#endif //GAME_TERM_SWITCHFIGHTMODECOMMAND_H