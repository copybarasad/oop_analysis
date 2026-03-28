#ifndef POINT_H
#define POINT_H
#include <istream>
#include <ostream>

enum State{
    lock,
    unlock,
    tower,
    build,
    slow,
    trap
};

class Point
{
public:
    Point(int x, int y, State opt);
    Point();
    int get_point_x() const;
    int get_point_y() const;
    State get_point_opt() const;

    void set_option(State opt);
    void set_point_x(int dx);
    void set_point_y(int dy);
    void serialize(std::ostream& os) const ;
    void deserialize(std::istream& is) ;

private:
    int x;
    int y;
    State option;
};

#endif // POINT_H
