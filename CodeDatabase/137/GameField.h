#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "Cell.h"
#include <vector>
#include <iostream>

class GameField {
public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    ~GameField();

    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;

    int GetWidth() const;
    int GetHeight() const;

    Cell& GetCell(int x, int y);
    const Cell& GetCell(int x, int y) const;

    bool IsValidPosition(int x, int y) const;
    bool IsPassable(int x, int y) const;

    void Serialize(std::ostream& stream) const;
    void Deserialize(std::istream& stream);

private:
    void InitializeCells();
    void SerializeCell(std::ostream& stream, const Cell& cell) const;
    void DeserializeCell(std::istream& stream, Cell& cell);

    int width_;
    int height_;
    std::vector<std::vector<Cell>> cells_;
};

#endif // GAME_FIELD_H
