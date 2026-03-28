#pragma once

enum class CellType { EMPTY, ENEMY, PLAYER, WALL, SLOW, BUILDING, TRAP, TOWER };

class Cell {
public:
    Cell();

    CellType GetType() const;
    void SetType(CellType new_type);

private:
    CellType type_;
};
