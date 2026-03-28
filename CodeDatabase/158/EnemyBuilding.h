#pragma once

#include "Core/Coordinates.h"
#include "Core/Map.h"
#include <string>
#include <sstream>

class EnemyBuilding 
{
protected:
    Coordinates XY_coord;
    std::string Body;
public:
    EnemyBuilding(int X_coord = 0, int Y_coord = 0) : XY_coord(X_coord, Y_coord) {}
    virtual ~EnemyBuilding() = default;
    
    int getX_coord() const;
    int getY_coord() const;
    std::string getBody() const;
    virtual void setTile(Map& game_map);
    
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};