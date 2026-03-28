#include "EnemyTower.h"

EnemyTower::EnemyTower(int X_coord, int Y_coord, int range) 
    : EnemyBuilding(X_coord, Y_coord), HP(4), maxHP(4), range(range), attackCD(4), currentCD(0)
{ 
    Body = "⯘ "; 
}

int EnemyTower::getHP() const { return HP; }
void EnemyTower::take_damage(int damage) { HP -= damage; if(HP <= 0) HP = 0; }
bool EnemyTower::isAlive() const { return HP > 0; }

std::vector<Coordinates> EnemyTower::getAttackCoordinates() const 
{
    std::vector<Coordinates> targets;
    for(int dx = -range; dx <= range; dx++)
        for(int dy = -range; dy <= range; dy++)
            if(dx == 0 && dy == 0) continue;
            else targets.push_back(Coordinates(XY_coord.getX_coord() + dx, XY_coord.getY_coord() + dy));
    return targets;
}

bool EnemyTower::updateAndAttack() 
{
    if(currentCD > 0) 
    { 
        currentCD--; 
        return false; 
    }
    currentCD = attackCD;
    return true;
}

std::string EnemyTower::serialize() const 
{
    std::stringstream ss;
    ss << "TOWER " << XY_coord.getX_coord() << " " << XY_coord.getY_coord() << " " 
       << HP << " " << range << " " << attackCD << " " << currentCD;
    return ss.str();
}

void EnemyTower::deserialize(const std::string& data) 
{
    std::stringstream ss(data);
    std::string type;
    ss >> type;
    int x, y;
    ss >> x >> y >> HP >> range >> attackCD >> currentCD;
    XY_coord.setXY_coord(x, y);
    maxHP = 4;
}