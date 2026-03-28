#include "position.h"
#include <algorithm>

Position::Position(int x, int y) : x(x), y(y) {}

int Position::get_x() const { return x; }
int Position::get_y() const { return y; }

void Position::set_x(int new_x) { x = new_x; }
void Position::set_y(int new_y) {y = new_y; }

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

int Position::find_distance_to(Position target_position) const {
    int delta_x = std::abs(target_position.get_x() - x);
    int delta_y = std::abs(target_position.get_y() - y);
    return std::max(delta_x, delta_y);
}

