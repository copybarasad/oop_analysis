#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Cell.h"
#include <vector>
#include <utility>

class GameField {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> grid;
    int playerX, playerY;

public:
    GameField(int w, int h);
    ~GameField();
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    friend void swap(GameField& first, GameField& second) noexcept;

    int getWidth() const;
    int getHeight() const;
    void setPlayerPosition(int x, int y);
    std::pair<int, int> getPlayerPosition() const;
    bool movePlayer(int newX, int newY);
    char getCellContent(int x, int y) const;
    void setCellContent(int x, int y, char content);
    bool isCellEmpty(int x, int y) const;
    void clearCell(int x, int y);
    void clearAllCells();
    bool isValidPosition(int x, int y) const;
};

#endif
