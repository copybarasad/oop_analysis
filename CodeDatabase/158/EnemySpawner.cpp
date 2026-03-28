#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(int X_coord, int Y_coord, int spawnCD) 
    : EnemyBuilding(X_coord, Y_coord), spawnCD(spawnCD), currentCD(0) 
{ 
    Body = "಄ "; 
}

bool EnemySpawner::updateAndSpawn() 
{
    if (currentCD <= 0) 
    { 
        currentCD = spawnCD; 
        return true; 
    }
    else 
    { 
        currentCD--; 
        return false; 
    }
}

std::string EnemySpawner::serialize() const 
{
    std::stringstream ss;
    ss << "SPAWNER " << XY_coord.getX_coord() << " " << XY_coord.getY_coord() << " " 
       << spawnCD << " " << currentCD;
    return ss.str();
}

void EnemySpawner::deserialize(const std::string& data) 
{
    std::stringstream ss(data);
    std::string type;
    ss >> type;
    int x, y;
    ss >> x >> y >> spawnCD >> currentCD;
    XY_coord.setXY_coord(x, y);
}