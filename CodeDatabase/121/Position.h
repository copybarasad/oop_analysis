#ifndef POSITION_H
#define POSITION_H

#include <utility>
#include <string>
#include <cmath>

using Position = std::pair<int, int>;

namespace PositionUtils {
    inline std::string toString(const Position& pos) {
        return "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ")";
    }
    
    inline double calculateDistance(const Position& pos1, const Position& pos2) {
        int dx = pos1.first - pos2.first;
        int dy = pos1.second - pos2.second;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    inline bool isValid(const Position& pos, int maxWidth, int maxHeight) {
        return pos.first >= 0 && pos.first < maxWidth && 
               pos.second >= 0 && pos.second < maxHeight;
    }
}

#endif