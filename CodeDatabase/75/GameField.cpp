#include "GameField.h"
#include <iostream>

GameField::GameField(int width, int height) 
    : m_width(width), m_height(height) {
    initialize();
}
GameField::GameField(const GameField& other) 
    : m_width(other.m_width), m_height(other.m_height) {
    
    m_grid.resize(m_height);
    for (int y = 0; y < m_height; ++y) {
        m_grid[y].resize(m_width);
        for (int x = 0; x < m_width; ++x) {
            m_grid[y][x] = Cell(other.m_grid[y][x].getType());
        }
    }
}
GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        m_width = other.m_width;
        m_height = other.m_height;
        
        m_grid.resize(m_height);
        for (int y = 0; y < m_height; ++y) {
            m_grid[y].resize(m_width);
            for (int x = 0; x < m_width; ++x) {
                m_grid[y][x] = Cell(other.m_grid[y][x].getType());
            }
        }
    }
    return *this;
}
void GameField::initialize() {
    m_grid.resize(m_height, std::vector<Cell>(m_width));
    
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1) {
                m_grid[y][x] = Cell(Cell::WALL);
            } else {
                m_grid[y][x] = Cell(Cell::EMPTY);
            }
        }
    }
}

void GameField::draw(int playerX, int playerY, const std::vector<Enemy*>& enemies) const {
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (x == playerX && y == playerY) {
                std::cout << 'P';
            } else {
                bool enemyDrawn = false;
                for (Enemy* enemy : enemies) {
                    if (enemy->isAlive() && enemy->getX() == x && enemy->getY() == y) {
                        std::cout << enemy->getSymbol();
                        enemyDrawn = true;
                        break;
                    }
                }
                if (!enemyDrawn) {
                    std::cout << m_grid[y][x].getChar();
                }
            }
        }
        std::cout << '\n';
    }
}

Cell& GameField::getCell(int x, int y) {
    return m_grid[y][x];
}
const Cell& GameField::getCell(int x, int y) const {
    return m_grid[y][x];
}
bool GameField::isInBounds(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

int GameField::getWidth() const {
    return m_width;
}

int GameField::getHeight() const {
    return m_height;
}