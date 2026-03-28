#ifndef RANGED_ATTACK_COMMAND_H
#define RANGED_ATTACK_COMMAND_H

#include "command.h"

class RangedAttackCommand : public Command {
public:
    void execute(Game& game) override;
    const char* getDescription() const override { return "Ranged Attack"; }
};

#endif