#include "Cell.h"

Cell::Cell(int x, int y, bool walkable) : coords{ x, y }, walkable(walkable) {}

json Cell::serialize() const {
    return json{ {"x", coords.x}, {"y", coords.y}, {"walkable", walkable} };
}

void Cell::deserialize(const json& j) {
    coords.x = j.at("x").get<int>();
    coords.y = j.at("y").get<int>();
    walkable = j.at("walkable").get<bool>();
}
