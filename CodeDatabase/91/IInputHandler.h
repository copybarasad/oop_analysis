#ifndef INPUT_IINPUTHANDLER_H
#define INPUT_IINPUTHANDLER_H

#include <memory>
#include "../Commands/ICommand.h"

class IInputHandler {
    public:
        virtual ~IInputHandler() = default;
        virtual std::unique_ptr<ICommand> getCommand() = 0;
};

#endif