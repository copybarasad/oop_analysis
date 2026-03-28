#include "point.hpp"

bool Point::operator==(const Point other) const
{
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point other) const
{
    return !(*this == other);
}

std::vector<Point> Point::get_neighbors(int width, int height)
{
    std::vector<Point> neighbors;
    if (x > 0)
        neighbors.push_back({x - 1, y});
    if (x < width - 1)
        neighbors.push_back({x + 1, y});
    if (y > 0)
        neighbors.push_back({x, y - 1});
    if (y < height - 1)
        neighbors.push_back({x, y + 1});
    return neighbors;
}

Point Point::operator+(const Point &other)
{
    return Point{x + other.x, y + other.y};
}

Point Point::operator-(const Point &other)
{
    return Point{x - other.x, y - other.y};
}

int Point::len2()
{
    return x * x + y * y;
}

bool Point::is_in_rect(Point corner1, Point corner2) {
    return x >= corner1.x && y >= corner1.y && x < corner2.x && y < corner2.y;
}

bool Point::operator<(const Point other) const {
    if (x < other.x) return true;
    if (x == other.x && y < other.y) return true;
    return false;
}

bool Point::inside_field(int width, int height) {
    return (x >= 0 && y >= 0 && x < width && y < height);
}