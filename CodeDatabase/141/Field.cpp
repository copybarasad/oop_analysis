#include "Field.h"
#include "GameConstants.h"
#include <stdexcept>
#include <chrono>

Field::Field(int fieldWidth, int fieldHeight)
    : width(fieldWidth), height(fieldHeight),
      randomGenerator(std::chrono::steady_clock::now().time_since_epoch().count()) {
    if (fieldWidth < GameConstants::MIN_FIELD_SIZE ||
        fieldWidth > GameConstants::MAX_FIELD_SIZE ||
        fieldHeight < GameConstants::MIN_FIELD_SIZE ||
        fieldHeight > GameConstants::MAX_FIELD_SIZE) {
        throw std::invalid_argument("Размер поля должен быть от " +
                                    std::to_string(GameConstants::MIN_FIELD_SIZE) + "x" +
                                    std::to_string(GameConstants::MIN_FIELD_SIZE) + " до " +
                                    std::to_string(GameConstants::MAX_FIELD_SIZE) + "x" +
                                    std::to_string(GameConstants::MAX_FIELD_SIZE));
    }

    grid.resize(height, std::vector<Cell>(width));
    initializeField();
}

Field::Field(const Field &other) : width(other.width), height(other.height) {
    copyFrom(other);
}

Field::Field(Field &&other) noexcept
    : grid(std::move(other.grid)), width(other.width), height(other.height) {
    other.width = 0;
    other.height = 0;
}

Field &Field::operator=(const Field &other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        copyFrom(other);
    }
    return *this;
}

Field &Field::operator=(Field &&other) noexcept {
    if (this != &other) {
        grid = std::move(other.grid);
        width = other.width;
        height = other.height;
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

int Field::getWidth() const {
    return width;
}

int Field::getHeight() const {
    return height;
}

const Cell &Field::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Координаты вне поля");
    }
    return grid[y][x];
}

void Field::setCell(int x, int y, CellType type) {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Координаты вне поля");
    }
    grid[y][x].setType(type);
}

bool Field::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Field::isPassable(int x, int y) const {
    return isValidPosition(x, y) && getCell(x, y).isPassable();
}

void Field::initializeField() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            setCell(x, y, CellType::EMPTY);
        }
    }

    int obstacleCount = (width * height) * GameConstants::OBSTACLE_PERCENTAGE;
    int placedObstacles = 0;

    for (int i = 0; i < obstacleCount * 2 && placedObstacles < obstacleCount; ++i) {
        int x = randomInt(1, width - 2);
        int y = randomInt(1, height - 2);

        int emptyNeighbors = 0;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx;
                int ny = y + dy;
                if (isValidPosition(nx, ny) && getCell(nx, ny).getType() == CellType::EMPTY) {
                    emptyNeighbors++;
                }
            }
        }

        if (emptyNeighbors >= GameConstants::MIN_EMPTY_NEIGHBORS) {
            setCell(x, y, CellType::OBSTACLE);
            placedObstacles++;
        }
    }

    int slowCount = (width * height) * GameConstants::SLOW_CELL_PERCENTAGE;
    for (int i = 0; i < slowCount; ++i) {
        int x = randomInt(0, width - 1);
        int y = randomInt(0, height - 1);

        if (getCell(x, y).getType() == CellType::EMPTY) {
            setCell(x, y, CellType::SLOW);
        }
    }

    std::vector<std::pair<int, int> > guaranteedEmptyAreas = {
        {1, 1}, {1, 2}, {2, 1}, {2, 2},
        {width - 2, height - 2}, {width - 3, height - 2},
        {width - 2, height - 3}, {width - 3, height - 3},
        {width / 2, height / 2}, {width / 2 + 1, height / 2},
        {width / 2, height / 2 + 1}, {width / 2 + 1, height / 2 + 1}
    };

    for (const auto &area: guaranteedEmptyAreas) {
        if (isValidPosition(area.first, area.second)) {
            setCell(area.first, area.second, CellType::EMPTY);
        }
    }
}

void Field::copyFrom(const Field &other) {
    grid.resize(other.height);
    for (int i = 0; i < other.height; ++i) {
        grid[i] = other.grid[i];
    }
}

int Field::randomInt(int min, int max) const {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(randomGenerator);
}
