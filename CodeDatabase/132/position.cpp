#include "position.hpp"

bool Position::operator==(const Position &other) const {
    return (x == other.x) && (y == other.y);
}

bool Position::operator!=(const Position &other) const {
    return !(*this == other);
}

bool Position::operator<(const Position &other) const {
    if (x == other.x) {
        return y < other.y;
    }
    return x < other.x;
}

int Position::manhattan_distance(const Position &from, const Position &to) {
    return abs(from.x - to.x) + abs(from.y - to.y);
}

int Position::euclidean_sq_distance(const Position &from, const Position &to) {
    int dx = from.x - to.x;
    int dy = from.y - to.y;
    return dx * dx + dy * dy;
}