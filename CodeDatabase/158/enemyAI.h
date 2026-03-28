#pragma once

#include "Core/Coordinates.h"
#include "InputController.h"
#include <cstdlib>

inline Direction enemyAI(Coordinates enemy_pos, Coordinates player_pos) 
{
    int dx = player_pos.getX_coord() - enemy_pos.getX_coord();
    int dy = player_pos.getY_coord() - enemy_pos.getY_coord();
    
    if (rand()%100 < 69) 
    {
        if (abs(dx) > abs(dy)) 
        {
            if (dx > 0) return Direction::RIGHT;
            else return Direction::LEFT;
        } 
        else 
        {
            if (dy > 0) return Direction::DOWN;
            else return Direction::UP;
        }
    } 
    else 
    {
        int random_dir = rand() % 4;
        switch(random_dir) 
        {
            case 0: return Direction::UP;
            case 1: return Direction::DOWN;
            case 2: return Direction::LEFT;
            case 3: return Direction::RIGHT;
            default: return Direction::KNOWHERE;
        }
    }
}