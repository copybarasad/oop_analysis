#pragma once
#include "../IGameController.hpp"
#include "../InputController.hpp"

class IVisualizer {
public:
    virtual void setInputController(InputController* inputController) = 0;
    virtual void setGameController(IGameController* gameController) = 0;
    virtual void display() = 0;
};
