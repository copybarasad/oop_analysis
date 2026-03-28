#ifndef CELL_H_
#define CELL_H_


enum class CellType {
    kEmpty,
    kObstacle,
    kSlow,
    kPlayer,
    kEnemy,
    kBuilding,
    kTrap,
    kTower,
    kAlly
};


class Cell {
public:
    Cell() = default;
    explicit Cell(CellType type) : type_(type) {}
    CellType GetType() const { return type_; }
    void SetType(CellType type) { type_ = type; }


private:
    CellType type_ = CellType::kEmpty;
};


#endif // CELL_H_
