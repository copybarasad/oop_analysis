#ifndef ICOMMAND
#define ICOMMAND

class ICommand
{
public:
    virtual bool execute() = 0;
};

#endif