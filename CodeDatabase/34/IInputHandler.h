#ifndef GAME_IINPUTHANDLER_H
#define GAME_IINPUTHANDLER_H

#include <memory>
#include "../commands/ICommand.h"

namespace Game {

// Интерфейс для обработчиков ввода
class IInputHandler {
public:
    virtual ~IInputHandler() = default;
    virtual std::unique_ptr<ICommand> getCommand() = 0;
};

}

#endif
