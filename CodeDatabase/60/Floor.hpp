#pragma once
#include <Entity.hpp>
#include <Constants.hpp>

class Floor : public Entity {
    char sprite = FLOOR;
    
public:
    Floor(int x, int y);
    virtual void draw();
};