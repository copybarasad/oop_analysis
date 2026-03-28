#ifndef POINT_HPP
#define POINT_HPP

#include <fstream>
#include <vector>

struct Point {
    int x, y;
    bool operator==(const Point other) const;
    bool operator!=(const Point other) const;
    std::vector<Point> get_neighbors(int width, int height);

    Point operator+(const Point &other);
    Point operator-(const Point &other);
    bool inside_field(int width, int height);
    int len2();
    bool is_in_rect(Point corner1, Point corner2);
    bool operator<(const Point other) const;

    std::string str()
    {
        return "point (" + std::to_string(x) + ", " + std::to_string(y) +")";
    }
};

#endif
