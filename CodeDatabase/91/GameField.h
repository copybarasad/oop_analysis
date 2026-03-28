#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "Cell.h"
#include "Position.h"
#include <vector>
#include <memory>
#include <random>
#include <set>

class GameField {
    private:
        int width;
        int height;
        std::vector<std::vector<Cell>> cells;

        bool isValidPosition(const Position& position) const;
        void initializeField(const std::set<Position>* reserved = nullptr);

    public:
        GameField(const GameField& other);
        GameField(GameField&& other) noexcept;
        GameField& operator=(const GameField& other);
        GameField& operator=(GameField&& other) noexcept;
        explicit GameField(int fieldWidth, int fieldHeight,
                   const std::vector<Position>& reservedPositions = {});

        ~GameField() = default;

        // === Basic methods ===
        bool isCellPassable(const Position& position) const;
        bool canMoveTo(const Position& newPosition) const;

        // === Getters ===
        int getWidth() const;
        int getHeight() const;

        const Cell& getCell(const Position& position) const;
        Cell& getCell(const Position& position);

        // === Validation ===
        bool isPositionValid(const Position& position) const;

        // === JSON ===
        void loadState(const nlohmann::json& j);

        const std::vector<std::vector<Cell>>& getCells() const;
};

#endif