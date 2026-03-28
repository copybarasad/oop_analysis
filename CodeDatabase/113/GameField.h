#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <iostream>
#include "Cell.h"
#include "GameObject.h"

// Предварительные объявления
class Player;

class GameField {
private:
    std::vector<std::vector<char>> m_grid;
    std::vector<std::vector<Cell>> m_cells;
    int m_width;
    int m_height;

public:
    GameField() : m_width(0), m_height(0) {}
    GameField(int width, int height) { initialize(width, height); }

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    char getCell(int x, int y) const {
        if (!isWithinBounds(x, y)) return ' ';
        return m_grid[y][x];
    }

    void setCell(int x, int y, char value) {
        if (!isWithinBounds(x, y)) return;
        m_grid[y][x] = value;

        // Обновляем проходимость
        if (value == '#') {
            m_cells[y][x].setPassable(false);
        }
        else {
            m_cells[y][x].setPassable(true);
        }
    }

    void initialize(int width, int height) {
        m_width = width;
        m_height = height;
        m_grid.resize(height, std::vector<char>(width, ' '));
        m_cells.resize(height, std::vector<Cell>(width, Cell(true)));
    }

    void clear() {
        for (auto& row : m_grid) {
            std::fill(row.begin(), row.end(), ' ');
        }
        for (auto& row : m_cells) {
            for (auto& cell : row) {
                cell = Cell(true);
            }
        }
    }

    bool isWithinBounds(int x, int y) const {
        return x >= 0 && x < m_width && y >= 0 && y < m_height;
    }

    bool isValidPosition(int x, int y) const {
        return isWithinBounds(x, y);
    }

    bool isCellPassable(int x, int y) const {
        if (!isWithinBounds(x, y)) return false;
        return m_cells[y][x].isPassable();
    }

    bool hasTrap(int x, int y) const {
        if (!isWithinBounds(x, y)) return false;
        return m_cells[y][x].hasTrap();
    }

    int getTrapDamage(int x, int y) const {
        if (!isWithinBounds(x, y)) return 0;
        return m_cells[y][x].getTrapDamage();
    }

    void removeTrap(int x, int y) {
        if (isValidPosition(x, y)) {
            m_cells[y][x].removeTrap();
        }
    }

    // Метод для GameObject
    void triggerTrap(int x, int y, GameObject& target) {
        if (hasTrap(x, y)) {
            int damage = getTrapDamage(x, y);
            target.takeDamage(damage);
            std::cout << "Trap triggered! " << damage << " damage to target at ("
                << x << "," << y << ")" << std::endl;
            removeTrap(x, y);
        }
    }

    void triggerTrap(int x, int y, Player& player);

    void placeTrap(int x, int y, int damage) {
        if (isValidPosition(x, y) && isCellPassable(x, y)) {
            m_cells[y][x].setTrap(damage);
            std::cout << "*** Trap placed at (" << x << "," << y << ") with damage " << damage << std::endl;
        }
    }

    void printField() const {
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                std::cout << m_grid[y][x] << " ";
            }
            std::cout << std::endl;
        }
    }

    void clearAll() {
        for (auto& row : m_grid) {
            std::fill(row.begin(), row.end(), ' ');
        }
        for (auto& row : m_cells) {
            for (auto& cell : row) {
                cell = Cell(true); // Все клетки становятся проходимыми
            }
        }
    }
};

#endif // GAMEFIELD_H