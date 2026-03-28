#ifndef SPAWNBUILDING_H
#define SPAWNBUILDING_H
#include "point.h"
#include "enemy.h"
#include <ostream>
#include <istream>

class SpawnBuilding
{
public:
    SpawnBuilding(int x, int y);
    ~SpawnBuilding();
    int get_coord_x() const;
    int get_coord_y() const;
    void spawn(Enemy& comp, int x, int y);
    void serialize(std::ostream& os) const ;
    void deserialize(std::istream& is) ;

private:
    int coord_x;
    int coord_y;
    Point* building_position;
};

#endif // SPAWNBUILDING_H
