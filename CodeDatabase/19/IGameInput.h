#pragma once
#include <functional>

#include "Action.h"


using ActionCallback = std::function<void(Action action)>;
using MouseClickCallback = std::function<void()>;
using StopCallback = std::function<void()>;

class IGameInput {
public:
    IGameInput() = default;

    virtual ~IGameInput() = default;

    virtual void tick() = 0;

    virtual bool init(const std::map<Action, std::string> &defaults, const std::map<Action, std::string> &actions,
                      ActionCallback callback, MouseClickCallback mouse_callback, StopCallback stopCallback) = 0;

    virtual void get_mouse_pos(int &x, int &y) const = 0;
};
