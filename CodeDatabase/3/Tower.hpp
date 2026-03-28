#pragma once
#include "types.hpp"
#include "GameObjects/Entities/interfaces.hpp"

class Tower {
public:
    bool onCoolDown();

    void setCoolDown(int cd);
    int getCoolDown();

    void addStep();
    int getSteps();
    void setSteps(int s);


protected:
    int coolDown;
    int steps = 0;
};
