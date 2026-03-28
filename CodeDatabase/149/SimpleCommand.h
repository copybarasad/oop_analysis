#ifndef SIMPLECOMMAND_H
#define SIMPLECOMMAND_H

#include "Command.h"

class SimpleCommand : public Command {
private:
    CommandType type;

public:
    SimpleCommand(CommandType commandType);
    CommandType getType() const override;
    bool isValid() const override;
};

#endif