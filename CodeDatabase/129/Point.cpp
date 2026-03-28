#include "Point.h"

Point::Point() {}

Point::Point(int a, int b){
    this->x = a;
    this->y = b;
}

bool Point::operator==(Point& other) {
    return x == other.x && y == other.y;
}

Point::~Point() {}
