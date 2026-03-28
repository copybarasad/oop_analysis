#ifndef CELL_H
#define CELL_H

class Entity;

enum CellType {PASSABLE = '.', IMPASSABLE = '$', TRAPS = '*'};

class Cell {
private:
    CellType type;
    Entity* entity = nullptr;

public:
    explicit Cell(CellType type = PASSABLE);

    CellType getType() const;
    char getSymbol() const;

    void setEntity(Entity* e) { entity = e; }
    Entity* getEntity() const { return entity; }
    bool hasEntity() const { return entity != nullptr; }
};

#endif