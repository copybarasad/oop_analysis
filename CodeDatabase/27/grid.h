#pragma once
#include <vector>
#include "point.h"

class Grid {
private:
    int length;
    int width;
    std::vector<Point> grd;

public:
    Grid(int length, int width);
    Grid(const Grid& p);
    Grid(Grid&& p) noexcept;
    Grid& operator=(const Grid& other);
    Grid& operator=(Grid&& other) noexcept;
    void print_grid() const;

    Point* getPoint(int x, int y);
    const Point* getPoint(int x, int y) const;
    int get_leng() const;
    int get_width() const;
};