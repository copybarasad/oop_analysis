#ifndef CELL_H
#define CELL_H

#include "Position.h"
#include "CellType.h"
#include "json.hpp"

class Cell {
    private:
        Position position;
        bool isPassable;
        CellType type;
        int trapDamage;

    public:
        Cell(int x, int y, bool passible = true, CellType cellType = CellType::EMPTY);

        // === Getters ===
        Position getPosition() const;
        bool getIsPassable() const;
        CellType getType() const;
        int getTrapDamage() const;

        // === Setters ===
        void setIsPassable(bool passable);
        void setType(CellType cellType);
        void setTrapDamage(int damage);

        bool hasTrap() const;
        bool hasTower() const;
        void activateTrap();
};

// === JSON ===
inline void to_json(nlohmann::json& j, const Cell& c) {
    j = {
        {"position", c.getPosition()},
        {"isPassable", c.getIsPassable()},
        {"type", c.getType()},
        {"trapDamage", c.getTrapDamage()}
    };
}

inline void from_json(const nlohmann::json& j, Cell& c) {
    Position pos;
    j.at("position").get_to(pos);
    bool isPassable = j.at("isPassable").get<bool>();
    CellType type = j.at("type").get<CellType>();
    int trapDamage = j.at("trapDamage").get<int>();

    if (type == CellType::TRAP && trapDamage <= 0) {
         throw std::invalid_argument("CellType TRAP must have trapDamage > 0");
    }
    if (type != CellType::TRAP && trapDamage != 0) {
         throw std::invalid_argument("CellType not TRAP must have trapDamage == 0, got: " + std::to_string(trapDamage));
    }
    if (type == CellType::TRAP && !isPassable) {
        throw std::invalid_argument("CellType TRAP must be passable, isPassable cannot be false");
    }

    Cell temp_cell(pos.x, pos.y, (type == CellType::TRAP) ? true : isPassable, type);

    if (temp_cell.getType() != type) {
        throw std::runtime_error("Cell deserialization failed: type mismatch after construction.");
    }

    if (type == CellType::TRAP) {
        temp_cell.setTrapDamage(trapDamage);
    }
    c = std::move(temp_cell);
}

#endif