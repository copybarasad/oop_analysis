#pragma once
#include <Entity.hpp>
#include <Constants.hpp>

class Wall : public Entity {
    char sprite = WALL;
    
public:
    Wall(int x, int y);
    virtual void draw() override;
};