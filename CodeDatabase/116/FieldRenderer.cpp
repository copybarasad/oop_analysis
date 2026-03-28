#include "FieldRenderer.h"

const std::string FieldRenderer::COLOR_RESET = "\033[0m";
const std::string FieldRenderer::COLOR_PLAYER = "\033[1;32m";
const std::string FieldRenderer::COLOR_ENEMY = "\033[1;31m";
const std::string FieldRenderer::COLOR_WALL = "\033[1;90m";
const std::string FieldRenderer::COLOR_EMPTY = "\033[1;34m";
const std::string FieldRenderer::COLOR_SLOWING = "\033[1;36m";
const std::string FieldRenderer::COLOR_TOWER = "\033[1;33m";
const std::string FieldRenderer::COLOR_TRAP = "\033[1;35m";

FieldRenderer::CellDisplay FieldRenderer::getCellSymbolColor(const GameField& field,int x, int y) const {
    const Cell& cell = field.getCell(x, y);
    if (cell.getType() == Cell::CellType::PLAYER) {
        return CellDisplay('P', COLOR_PLAYER);
    } else if (cell.getType() == Cell::CellType::ENEMY) {
        return CellDisplay('E', COLOR_ENEMY);
    } else if (cell.getType() == Cell::CellType::ALLY) {
        return CellDisplay('A', COLOR_PLAYER);
    } else if (cell.getType() == Cell::CellType::BUILDING) {
        return CellDisplay('T', COLOR_TOWER);
    } else if (cell.getType() == Cell::CellType::FORTRESS) {
        return CellDisplay('F', COLOR_TOWER);
    } else if (cell.getType() == Cell::CellType::WALL) {
        return CellDisplay('#', COLOR_WALL);
    } else if (cell.getType() == Cell::CellType::TRAP) {
        return CellDisplay('*', COLOR_TRAP);
    } else if (cell.isSlowing()) {
        return CellDisplay('O', COLOR_SLOWING);
    } else {
        return CellDisplay('.', COLOR_EMPTY);
    }
}

void FieldRenderer::printFieldWithColors(const GameField& field) const{
    system("clear");
    std::cout << "=== ИГРОВОЕ ПОЛЕ ===" << std::endl;
    std::cout << "Размер: " << field.getWidth() << "x" << field.getHeight() << std::endl;
    std::cout << std::endl;
    
    std::cout << "Легенда: ";
    std::cout << COLOR_PLAYER << "P" << COLOR_RESET << " - Игрок, ";
    std::cout << COLOR_PLAYER << "A" << COLOR_RESET << " - Союзник, ";
    std::cout << COLOR_ENEMY << "E" << COLOR_RESET << " - Враг, ";
    std::cout << COLOR_WALL << "#" << COLOR_RESET << " - Стена, ";
    std::cout << COLOR_EMPTY << "O" << COLOR_RESET << " - Замедление, ";
    std::cout << COLOR_TOWER << "T" << COLOR_RESET << " - Башня, ";
    std::cout << COLOR_TRAP << "*" << COLOR_RESET << " - Ловушка, ";
    std::cout << COLOR_EMPTY << "." << COLOR_RESET << " - Пусто";
    std::cout << std::endl << std::endl;

    std::cout << "  ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << (x < 10 ? " " : "") << x << " ";
    }
    std::cout << std::endl;
    std::cout << "  ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "---";
    }
    std::cout << std::endl;
    
    for (int y = 0; y < field.getHeight(); ++y) {
        std::cout << (y < 10 ? " " : "") << y << "|";
        
        for (int x = 0; x < field.getWidth(); ++x) {
            CellDisplay display = getCellSymbolColor(field,x, y);
            std::cout << display.color << display.symbol << COLOR_RESET << "  ";
        }
        std::cout << "|" << std::endl;
    }
    
    std::cout << "  ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "---";
    }
    std::cout << std::endl;
}
