#ifndef IINPUTHANDLER_H
#define IINPUTHANDLER_H

#include "ICommand.h"
#include <memory>

class IInputHandler {
public:
    virtual ~IInputHandler() = default;
    virtual std::unique_ptr<ICommand> getCommand() = 0;
};

#endif // IINPUTHANDLER_H