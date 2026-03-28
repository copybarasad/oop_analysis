#ifndef CELL_H
#define CELL_H

#include <string>
#include "ITrapSpell.h"

class Cell {
private:
    bool m_isPassable;
    bool m_hasTrap;
    int m_trapDamage;
    std::string m_trapType; // Новое поле для типа ловушки

public:
    Cell(bool isPassable = true);
    bool isPassable() const;
    void setPassable(bool passable);
    bool hasTrap() const;
    void setTrap(int damage);
    void setTrap(const ITrapSpell& trapSpell);
    void removeTrap();
    int getTrapDamage() const;
    std::string getTrapType() const;
};

#endif // CELL_H