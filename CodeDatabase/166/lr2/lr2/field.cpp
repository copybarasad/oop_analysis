#include "field.hpp"
#include <stdexcept>

// простой индекс в массиве по (x, y)
static inline std::size_t idx(int x, int y, int w) noexcept {
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(w) + static_cast<std::size_t>(x);
}

// создаём поле и сразу проверяем допустимый размер
Field::Field(int w, int h)
    : w_(w), h_(h), grid_(static_cast<std::size_t>(w) * static_cast<std::size_t>(h)) {
    if (w < 10 || h < 10 || w > 25 || h > 25) {
        throw std::invalid_argument("Field size must be 10..25");
    }
}

// копирующий конструктор (глубокая копия)
Field::Field(const Field& o)
    : w_(o.w_), h_(o.h_), grid_(o.grid_) {}

// перемещающий конструктор
Field::Field(Field&& o) noexcept
    : w_(o.w_), h_(o.h_), grid_(std::move(o.grid_)) {
    o.w_ = 0; o.h_ = 0;
}

// копирующее присваивание
Field& Field::operator=(const Field& o) {
    if (this != &o) {
        w_ = o.w_;
        h_ = o.h_;
        grid_ = o.grid_;
    }
    return *this;
}

// перемещающее присваивание
Field& Field::operator=(Field&& o) noexcept {
    if (this != &o) {
        w_ = o.w_;
        h_ = o.h_;
        grid_ = std::move(o.grid_);
        o.w_ = 0; o.h_ = 0;
    }
    return *this;
}

// константный доступ к клетке с проверкой границ
const Cell& Field::at(int x, int y) const {
    if (!inBounds(x, y)) throw std::out_of_range("Field::at const");
    return grid_[idx(x, y, w_)];
}

// изменяемый доступ к клетке с проверкой границ
Cell& Field::at(int x, int y) {
    if (!inBounds(x, y)) throw std::out_of_range("Field::at");
    return grid_[idx(x, y, w_)];
}

// проверка, что (x, y) внутри поля
bool Field::inBounds(int x, int y) const noexcept {
    return (0 <= x && x < w_) && (0 <= y && y < h_);
}

// клетка проходима, если это не стена
bool Field::passable(int x, int y) const noexcept {
    return inBounds(x, y) && at(x, y).getType() != CellType::Wall;
}
