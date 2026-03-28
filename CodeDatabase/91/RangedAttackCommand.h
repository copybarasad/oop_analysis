#ifndef RANGED_ATTACK_COMMAND_H
#define RANGED_ATTACK_COMMAND_H

#include "ICommand.h"

class RangedAttackCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif