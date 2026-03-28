#pragma once
#include "../include/GameCycle.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(GameCycle& game) = 0;
};
