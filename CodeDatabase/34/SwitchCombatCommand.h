#ifndef GAME_SWITCHCOMBATCOMMAND_H
#define GAME_SWITCHCOMBATCOMMAND_H

#include "ICommand.h"

namespace Game {

class SwitchCombatCommand : public ICommand {
public:
    void execute(Game& game) override;
};

}

#endif