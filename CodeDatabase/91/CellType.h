#ifndef CELL_TYPE_H
#define CELL_TYPE_H

#include <stdexcept>
#include "json.hpp"

enum class CellType {
    EMPTY,
    WALL,
    TRAP,
    TOWER
};

// === JSON ===
inline void to_json(nlohmann::json& j, const CellType& ct) {
    switch (ct) {
        case CellType::EMPTY:
            j = "EMPTY";
            break;
        case CellType::WALL:
            j = "WALL";
            break;
        case CellType::TRAP:
            j = "TRAP";
            break;
        case CellType::TOWER:
            j = "TOWER";
            break;
        default:
            throw std::invalid_argument("Invalid CellType value for serialization");
    }
}

inline void from_json(const nlohmann::json& j, CellType& ct) {
    std::string s = j.get<std::string>();
    if (s == "EMPTY") {
        ct = CellType::EMPTY;
    } else if (s == "WALL") {
        ct = CellType::WALL;
    } else if (s == "TRAP") {
        ct = CellType::TRAP;
    } else if (s == "TOWER") {
        ct = CellType::TOWER;
    } else {
        throw std::invalid_argument("Invalid CellType string for deserialization: " + s);
    }
}

#endif