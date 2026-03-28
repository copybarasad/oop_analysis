#ifndef OOP_GAME_FIELD_H_
#define OOP_GAME_FIELD_H_

#include "cell.h"
#include <memory>

namespace rpg {
    class GameField final {
    public:
        // размеры поля
        static constexpr size_t kMinFieldSize = 10;
        static constexpr size_t kMaxFieldSize = 25;
        static constexpr size_t kDefaultFieldSize = 15;
        
        // плотность препятствий
        static constexpr double kDefaultImpassableDensity = 0.1;
        static constexpr double kDefaultSlowingDensity = 0.05;
        static constexpr double kMaxImpassableDensity = 0.4;
        static constexpr double kMaxSlowingDensity = 0.3;
        
        GameField(size_t width = kDefaultFieldSize, 
                  size_t height = kDefaultFieldSize,
                  double impassable_density = kDefaultImpassableDensity,
                  double slowing_density = kDefaultSlowingDensity);

        // ПРАВИЛО ПЯТИ
        ~GameField(); // 1. деструктор

        GameField(const GameField &other); // 2. конструктор копирования

        GameField &operator=(const GameField &other); // 3. оператор присваивания с копированием

        GameField(GameField &&other) noexcept; // 4. конструктор перемещения

        GameField &operator=(GameField &&other) noexcept; // 5. оператор присваивания с перемещением

        [[nodiscard]] size_t GetWidth() const;
        [[nodiscard]] size_t GetHeight() const;

        void ClearArea(size_t center_x, size_t center_y);

        [[nodiscard]] bool IsInBounds(size_t x, size_t y) const;

        [[nodiscard]] bool IsPassable(size_t x, size_t y) const;

        [[nodiscard]] bool IsSlowing(size_t x, size_t y) const;

        [[nodiscard]] const Cell &GetCell(size_t x, size_t y) const;

        void SetCellType(size_t x, size_t y, CellType type) const;

    private:
        size_t width_;
        size_t height_;
        Cell **cells_;

        void AllocateCells();

        void DeallocateCells();

        void DeepCopy(const GameField &other) const;

        void GenerateRandomCells(double impassable_density, double slowing_density) const;

        // Инвариант: width_ >= kMinFieldSize && width_ <= kMaxFieldSize
        // Инвариант: height_ >= kMinFieldSize && height_ <= kMaxFieldSize
        // Инвариант: cells_ != nullptr
    };
}

#endif
