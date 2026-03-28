#ifndef CELL_H
#define CELL_H

#include <cstddef>

class Unit; // forward declaration

// Тип содержимого клетки (terrain / special type)
enum class CellType {
    EMPTY,    // Пустая клетка
    PLAYER,   // Игрок
    ENEMY,    // Враг
    CLOSED,   // Закрытая клетка (непроходимая)
    FREEZE,   // Замедляющая клетка (проходимая)
    TOWER,     // Башня 
    E_TOWER,   // Вражеская башня
};

// Класс клетки игрового поля.
// Инвариант: если occupant != nullptr, то type == occupant->get_type()
class Cell {
public:
    Cell();
    Cell(CellType t);

    // Копирующий конструктор — occupant не копируем
    Cell(const Cell& other) : type(other.type), occupant(nullptr) {}

    // Оператор присваивания — occupant не копируем
    Cell& operator=(const Cell& other) {
        if (this != &other) {
            type = other.type;
            occupant = nullptr;
        }
        return *this;
    }

    // Тип/terrain
    void setType(CellType newType);
    CellType getType() const;

    // occupant: non-owning pointer (Cell НЕ ВЛАДЕЕТ объектом)
    void setOccupant(Unit* unit);
    void clearOccupant();
    Unit* getOccupant() const;
    bool hasOccupant() const;

    // Проверки
    bool isEmpty() const;
    bool isPassable() const;

private:
    CellType type;
    Unit* occupant = nullptr; // non-owning pointer
};

#endif // CELL_H

