#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "Command.h"
#include "InputConfig.h"

class ConsoleInputReader {
public:
    explicit ConsoleInputReader(const InputConfig& config);
    Command readCommand();

private:
    Command buildMoveCommand(CommandType type) const;
    Command buildAttackCommand();

private:
    InputConfig config_;
};

#endif


