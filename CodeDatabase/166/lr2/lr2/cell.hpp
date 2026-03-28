#ifndef CELL_HPP
#define CELL_HPP
#include <string>

// тип клетки поля (Пустая/стена/болото)
enum class CellType { Empty, Wall, Slow };

class Cell {
public:
    Cell() = default;
    explicit Cell(CellType t): type_(t) {}

    // геттер+сеттер типа клетки
    CellType getType() const noexcept { return type_; }
    void setType(CellType t) noexcept { type_ = t; }

    // получить символ для вывода
    char getSymbol() const noexcept {
        switch (type_) {
            case CellType::Empty: return '.';
            case CellType::Wall:  return '#';
            case CellType::Slow:  return '~';
        }
        return '?';
    }

private:
    // по умолчанию клетка пустая
    CellType type_{CellType::Empty};
};
#endif