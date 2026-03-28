#ifndef UTILS_POINT_H
#define UTILS_POINT_H

namespace Utils {

class Point {
public:
    int row;
    int column;

    Point(int initialRow = 0, int initialColumn = 0);

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
};

} 

#endif 
