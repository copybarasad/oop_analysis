#ifndef RPG_CELL_H
#define RPG_CELL_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum class CellType {
    CLEAR,
    SLOW,
    WALL
};

class Cell {
    CellType passability;
    bool occupied;
    unsigned int points;
public:
    Cell();

    explicit Cell(CellType type);

    explicit Cell(json& data);

    [[nodiscard]] json to_json() const;

    void setPassability(CellType passability);

    void entityDied(const unsigned int& points);

    void entityLeaved();

    [[nodiscard]] unsigned int entityPlaced();

    [[nodiscard]] CellType getPassability() const;

    [[nodiscard]] int getPoints() const;

    int collectPoints();

    [[nodiscard]] bool isOccupied() const;

    [[nodiscard]] bool isPassable() const;
};

#endif