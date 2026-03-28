#ifndef CELL_H
#define CELL_H

#include "../../Entities/Entity/Entity.hpp"

enum class CellType {
    NORMAL,
    BLOCKED,
    SLOWED
};

class Cell {
    private:
    size_t x, y;
    Entity* entity;
    CellType type;

    public:
    Cell () = default;
    Cell (size_t xValue, size_t yValue, CellType typeValue=CellType::NORMAL);
    Cell (const Cell& other);
    Cell (Cell&& other);

    Cell& operator=(const Cell& other);
    Cell& operator=(Cell&& other);

    void setEntity (Entity& placingEntity);
    void setType (CellType typeValue);

    size_t getX () const;
    size_t getY () const;
    template <typename Type>
    Type& getEntityAs () const {
        return dynamic_cast<Type&>(*entity);
    }
    Entity& getEntity () const;
    CellType getType () const;

    bool isEmpty () const;
    bool isPlayer () const;
    bool isEnemy () const;
    bool isBlocked () const;
    void clear ();
};

#endif