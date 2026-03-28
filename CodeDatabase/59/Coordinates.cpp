#include "Coordinates.h"

Coordinates Coordinates::operator+(const Coordinates& other) const {
    return { x + other.x, y + other.y 
};
}

bool Coordinates::operator==(const Coordinates& other) const {
    return x == other.x && y == other.y;
}

json Coordinates::toJson() const {
    return json{{"x", x}, {"y", y}};
}

void Coordinates::fromJson(const json& j) {
    j.at("x").get_to(x);
    j.at("y").get_to(y);
}