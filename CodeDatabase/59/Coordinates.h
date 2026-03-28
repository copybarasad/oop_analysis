#pragma once
#include <cmath>
#include "json.hpp"
using json = nlohmann::json;
struct Coordinates {
    int x;
    int y;

    Coordinates operator+(const Coordinates& other) const;
    bool operator==(const Coordinates& other) const;
    static float distanceBetween(const Coordinates& a, const Coordinates& b) {
        int dx = a.x - b.x;
        int dy = a.y - b.y;
        return (std::sqrt(dx * dx + dy * dy));
    }

    json toJson() const;
    void fromJson(const json& j);
};