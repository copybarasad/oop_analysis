#ifndef INPUTCOMMANDPROCESSOR_H
#define INPUTCOMMANDPROCESSOR_H

#include <memory>
#include "Command.h"

class InputCommandProcessor {
public:
    std::unique_ptr<Command> getCommand();
    Direction getDirection();
    int getSpellIndex(int maxSpells);
};

#endif