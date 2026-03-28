#include "Field.h"
#include <stdexcept>
#include <cstdlib>
#include <ctime>

Field::Field(int w, int l) : width(w), length(l) {
    if (w < 10 || w > 25 || l < 10 || l > 25) {
        throw std::invalid_argument("Размеры поля должны быть в диапазоне от 10x10 до 25x25.");
    }
    grid.resize(width, std::vector<Node>(length));
}

Field::Field(const Field& other) : width(other.width), length(other.length), grid(other.grid) {}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width = other.width;
        length = other.length;
        grid = other.grid;
    }
    return *this;
}

Field::Field(Field&& other) noexcept : width(other.width), length(other.length), grid(std::move(other.grid)) {
    other.width = 0;
    other.length = 0;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width = other.width;
        length = other.length;
        grid = std::move(other.grid);
        other.width = 0;
        other.length = 0;
    }
    return *this;
}

// !!! МЕТОДЫ getWidth И getLength УДАЛЕНЫ, ТАК КАК ОНИ УЖЕ ЕСТЬ В .H ФАЙЛЕ !!!

bool Field::areCoordinatesValid(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < length;
}

Node& Field::getNode(int x, int y) {
    if (!areCoordinatesValid(x, y)) {
        throw std::out_of_range("Координаты выходят за пределы поля.");
    }
    return grid[x][y];
}

const Node& Field::getNode(int x, int y) const {
    if (!areCoordinatesValid(x, y)) {
        throw std::out_of_range("Координаты выходят за пределы поля.");
    }
    return grid[x][y];
}

void Field::generateRandomObstacles(int percentage) {
    int totalCells = width * length;
    int obstacleCount = totalCells * percentage / 100;
    for (int i = 0; i < obstacleCount; ++i) {
        int x = rand() % width;
        int y = rand() % length;
        if (grid[x][y].getType() == Type::EMPTY) {
            grid[x][y].setType(Type::OBSTACLE);
        } else {
            i--;
        }
    }
}

void Field::save(std::ostream& os) const {
    os << width << " " << length << std::endl;
    for (int y = 0; y < length; ++y) {
        for (int x = 0; x < width; ++x) {
            os << static_cast<int>(grid[x][y].getType()) << " ";
        }
        os << std::endl;
    }
}

void Field::load(std::istream& is) {
    is >> width >> length;
    grid.assign(width, std::vector<Node>(length));
    
    for (int y = 0; y < length; ++y) {
        for (int x = 0; x < width; ++x) {
            int typeInt;
            is >> typeInt;
            grid[x][y].setType(static_cast<Type>(typeInt));
        }
    }
}

void Field::clear() {
    for(auto& row : grid) {
        for(auto& node : row) {
            node.setType(Type::EMPTY);
        }
    }
}