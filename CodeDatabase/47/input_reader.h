#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "command.h"

class IInputReader {
public:
    virtual ~IInputReader() = default;
    virtual Command readCommand() = 0;
};

#endif
