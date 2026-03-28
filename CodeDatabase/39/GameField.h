#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <cstddef>
#include <vector>

#include "Cell.h"

class GameField {
public:
    struct Size {
        std::size_t width;
        std::size_t height;
    };

    GameField(std::size_t width, std::size_t height);

    GameField(const GameField &other);

    GameField(GameField &&other) noexcept;

    GameField &operator=(const GameField &other);

    GameField &operator=(GameField &&other) noexcept;

    Size GetSize() const;

    bool IsInside(std::size_t x, std::size_t y) const;

    // --- Работа с террейном и существами ---
    Cell::Terrain GetTerrain(std::size_t x, std::size_t y) const;

    Cell::Occupant GetOccupant(std::size_t x, std::size_t y) const;

    int GetOccupantIndex(std::size_t x, std::size_t y) const;

    void SetTerrain(std::size_t x, std::size_t y, Cell::Terrain terrain);

    void SetOccupant(std::size_t x, std::size_t y,
                     Cell::Occupant occupant, int index);

    void ClearCell(std::size_t x, std::size_t y);

    bool FindPlayer(std::size_t &x, std::size_t &y) const;

    // --- Ловушки ---
    bool HasTrap(std::size_t x, std::size_t y) const;

    int GetTrapDamage(std::size_t x, std::size_t y) const;

    void PlaceTrap(std::size_t x, std::size_t y, int damage);

    void ClearTrap(std::size_t x, std::size_t y);

private:
    std::size_t width_;
    std::size_t height_;
    std::vector<Cell> cells_;

    std::size_t GetIndex(std::size_t x, std::size_t y) const;

    void InitializeTerrain();

    void EnsureInvariants() const;
};

#endif
