#ifndef LEON_H
#define LEON_H
#include "point.h"
#include <ostream>
#include <istream>

class Leon
{
public:
    Leon(int cx, int cy);
    ~Leon();
    int get_leon_x() const;
    int get_leon_y() const;
    int get_health_ability() const;
    void set_point_x(int x);
    void set_point_y(int y);
    void serialize(std::ostream& os) const ;
    void deserialize(std::istream& is) ;

private:
    int x;
    int y;
    int health_ability;
    Point* leon_position;
};

#endif // LEON_H
