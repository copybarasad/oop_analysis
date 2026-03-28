#ifndef POSITION_H
#define POSITION_H

#include <cmath>
#include <tuple>
#include "json.hpp"

struct Position {
    int x;
    int y;

    Position(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    bool operator<(const Position& other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }

    int distanceTo(const Position& other) const {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    bool isInRange(const Position& center, int range) const {
        return distanceTo(center) <= range;
    }

    Position withOffset(int dx, int dy) const {
        return Position(x + dx, y + dy);
    }
};

inline void to_json(nlohmann::json& j, const Position& p) {
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y}
    };
}

inline void from_json(const nlohmann::json& j, Position& p) {
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
}

#endif