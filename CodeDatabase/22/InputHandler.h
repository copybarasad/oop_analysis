#pragma once
#include "IInputHandler.h"


template <typename InputDevice>
class InputHandler : public IInputHandler
{
    InputDevice device;

public:
    template <typename... Args>
    InputHandler(Args &&...args) : device(std::forward<Args>(args)...) {}

    Command getCommand() override
    {
        return device.getCommand();
    }

    Command selectIndex(int max) override
    {
        return device.selectIndex(max);
    }

    Command readString() override{
        return device.readString();
    }
    Command selectPosition(Field &field, int radius, IGameView &render) override{
        return device.selectPosition(field, radius, render);
    }
    Command readOptions() override{
        return device.readOptions();
    }
};
