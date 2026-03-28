#ifndef SKIP_MOVE
#define SKIP_MOVE

#include "ICommand.h"

class SkipMove : public ICommand
{
public:
    bool execute() override
    {
        return true;
    }
};

#endif