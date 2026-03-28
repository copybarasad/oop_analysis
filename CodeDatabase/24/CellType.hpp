#pragma once
#include <cstdint>

// Типы клеток игрового поля
enum class CellType : uint8_t {
    EMPTY,
    OBSTACLE,
};
