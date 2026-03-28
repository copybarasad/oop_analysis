#pragma once
#include "../game/manager/Manager.h"
#include "../ui/GField.h"

class Graphic{
public:
    virtual void display(Manager manager) = 0;
    virtual void draw(GField gfield) = 0;
};
