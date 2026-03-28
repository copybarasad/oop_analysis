#ifndef GAMELB1_CELL_H
#define GAMELB1_CELL_H

enum class CellType {
    kEmpty,
    kObstacle,
    kPlayer,
    kEnemy,
    kBuilding,
    kSlowingTrap
};

class Cell {
public:
    Cell();

    void setType(CellType type);
    CellType getType() const;

private:
    CellType type_;
};

#endif //GAMELB1_CELL_H