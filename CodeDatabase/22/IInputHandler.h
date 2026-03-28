#pragma once
#include "command.h"
#include <cstddef>

class Field;
class IGameView;

class IInputHandler
{
public:
    virtual Command getCommand() = 0;
    virtual Command selectIndex(int max) = 0;
    virtual Command readString() = 0;
    virtual Command selectPosition(Field &field, int radius, IGameView &render) = 0;
    virtual Command readOptions() = 0;
    virtual ~IInputHandler() = default;
};
