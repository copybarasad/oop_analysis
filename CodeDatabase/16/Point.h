#ifndef POINT_H
#define POINT_H

struct Point {
    int x = 0;
    int y = 0;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

#endif