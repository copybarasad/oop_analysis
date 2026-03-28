#ifndef GAME_MAP_CELL_H
#define GAME_MAP_CELL_H

#include <iostream>

namespace Game {

enum class CellType {
    Empty,
    Impassable,
    Slowing
};

class MapCell {
public:
    MapCell(CellType type = CellType::Empty);
    ~MapCell();

    CellType getType() const;
    void setType(CellType newType);

    bool isPassable() const;
    bool isSlowing() const;

    void serialize(std::ostream& os) const;
    void deserialize(std::istream& is);

private:
    CellType cellType;
};

} 

#endif