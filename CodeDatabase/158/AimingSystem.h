#pragma once

#include <string>
#include <vector>
#include "Core/Coordinates.h"
#include "Core/Map.h"

class AimingSystem 
{
protected:
    std::string centerTile;
    std::vector<std::pair<Coordinates, std::string>> patternTiles;
    
public:
    AimingSystem(std::string center = "x ");
    void setDeathNoteAim();
    void setPsiStormAim(int AoE);
    void setTrapAim();
    void setDoppelgangerAim();
    void drawAim(Map& game_map, Coordinates target) const;
    void clearAim(Map& game_map, Coordinates target) const;
};