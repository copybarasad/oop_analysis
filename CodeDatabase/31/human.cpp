#include "human.hpp"
#include "field/field.hpp"

Human::Human(Field &field, Point position, int health)
    : Entity(field, position, health) {}

bool Human::move_to(Point offset) {
    Point target = position + offset;
    if (!target.inside_field(field.get_width(), field.get_height())) return false;
    if (field[target].get_type() == CellType::Impassible) return false;
    if (field[target].has_entity()) return false;
    field[position].set_entity(nullptr);
    position = target;
    field[position].set_entity(this);
    return true;
}

bool Human::left_motion() {
    return move_to({-1, 0});
}

bool Human::right_motion() {
    return move_to({1, 0});
}

bool Human::up_motion() {
    return move_to({0, -1});
}

bool Human::down_motion() {
    return move_to({0, 1});
}

Point Human::get_direction_offset(Human::Direction dir) {
    switch (dir) {
    case Direction::left: return Point { -1, 0 };
    case Direction::right: return Point { 1, 0 };
    case Direction::up: return Point { 0, -1 };
    case Direction::down: return Point { 0, 1 };
    default: return Point {0, 0};
    }
}
