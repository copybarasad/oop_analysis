#pragma once
#include <cstddef>

struct Location {
    size_t row;
    size_t col;
    
    Location(size_t r = 0, size_t c = 0) : row(r), col(c) {}
    
    bool operator==(const Location& other) const {
        return row == other.row && col == other.col;
    }
    
    bool isValid() const {
        return row != size_t(-1) && col != size_t(-1);
    }
    
    static Location invalid() {
        return Location(size_t(-1), size_t(-1));
    }
};