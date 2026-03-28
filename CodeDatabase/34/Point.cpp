#include "Point.h"

namespace Utils {

Point::Point(int initialRow, int initialColumn) : row(initialRow), column(initialColumn) {}

bool Point::operator==(const Point& other) const {
    return row == other.row && column == other.column;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

} 
