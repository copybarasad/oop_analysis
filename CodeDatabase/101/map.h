#ifndef MAP_H
#define MAP_H
#include "point.h"
#include "tower.h"
#include "constants.h"
#include "spawnbuilding.h"
#include "ostream"
#include "istream"

class Map
{
public:
    Map(const Tower& enemyTower, SpawnBuilding& bd, int x, int y);
    ~Map();
    Map(const Map& other);
    Map(Map&& other) noexcept;
    Map& operator=(const Map& other);
    Map& operator=(Map&& other) noexcept;
    const Point& get_point(int x, int y) const;
    int get_height() const;
    int get_width() const;
    Point& set_point(int x, int y);
    void serialize(std::ostream& os) const ;
    void deserialize(std::istream& is) ;


private:
    int height;
    int width;
    Point*** map_arr;
};

#endif // MAP_H
