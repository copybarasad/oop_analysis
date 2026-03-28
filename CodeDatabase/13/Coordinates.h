#ifndef GAME_COORDINATES_H
#define GAME_COORDINATES_H

#include <vector>

class Coordinates {
public:
    size_t x_, y_, layer_;
    Coordinates() : x_(0), y_(0), layer_(0) {
    };
    Coordinates(size_t x, size_t y, size_t layer) : x_(x), y_(y), layer_(layer) {
    };

    bool operator==(std::vector<Coordinates>::const_reference value) const = default;
};


#endif
