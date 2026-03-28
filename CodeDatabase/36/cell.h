#ifndef CELL_H
#define CELL_H

#include "cell_type.h"

class Cell {
public:
    Cell();
    CellType GetType() const;
    int GetEnemyIndex() const;
    int GetAllyIndex() const;
    int GetTrapDamage() const;

    void SetType(CellType t);
    void SetEnemyIndex(int idx);
    void SetAllyIndex(int idx);
    void SetTrapDamage(int dmg);

    bool IsPassable() const;
    bool ContainsEnemy() const;
    bool ContainsAlly() const;
    void Clear();

private:
    CellType type_;
    int enemy_index_;
    int ally_index_;
    int trap_damage_;
};

#endif