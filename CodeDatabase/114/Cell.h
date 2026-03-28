#ifndef CELL_H
#define CELL_H

#define TRAP_DAMAGE 30

enum class CellType {
    EMPTY,
    WALL,
    SLOW,
    TRAP
};

class Cell {
private:
    CellType type;
    int trapDamage;
    bool hasEntity;
public:
    Cell();
    
    CellType getType() const { return type; }
    bool getHasEntity() const { return hasEntity; }
    
    void setType(CellType newType);
    void setHasEntity(bool hasEntity);

    int getTrapDamage() const { return trapDamage; }
    void setTrapDamage(int trapDamage);

    bool canMoveTo() const;
};

#endif
