#include <Entity.hpp>
#include <Floor.hpp>

Floor::Floor(int x, int y) : Entity(x, y, Entity::Type::FLOOR) {}

void Floor::draw() {
    mvaddch(y, x, sprite);
}