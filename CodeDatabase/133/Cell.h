#ifndef CELL_H
#define CELL_H

enum class CellType {
    PASSABLE,
    IMPASSABLE,
    SLOWING,
    TRAP
};

class Cell {
private:
    CellType type;
    bool hasPlayer;
    bool hasEnemy;
    bool hasBuilding;
    bool hasTrap;
    bool hasAlly;

public:
    explicit Cell(CellType cellType = CellType::PASSABLE);

    CellType getType() const;
    void setType(CellType cellType);

    bool isPassable() const;
    bool hasPlayerOnCell() const;
    bool hasEnemyOnCell() const;
    bool hasBuildingOnCell() const;
    bool hasTrapOnCell() const;
    bool hasAllyOnCell() const;

    void setPlayerPresence(bool presence);
    void setEnemyPresence(bool presence);
    void setBuildingPresence(bool presence);
    void setTrapPresence(bool presence);
    void setAllyPresence(bool presence);
};

#endif