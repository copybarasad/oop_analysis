#include "Field.h"

Field::Field(int w, int h) : width(w), height(h), field(h, std::vector<Cell>(w)) {
    if (w < 10 || w > 25 || h < 10 || h > 25) {
        throw std::invalid_argument("Поле должно быть размерами от 10 на 10 до 25 на 25 клеток!");
    }
}

// генерация поля с рандомными параметрами
void Field::randomGenerate() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int s = width * height;
    int numWalls = s / 40 + (std::rand() % 3); // больше поле, больше стены очев
    for (int i = 0; i < numWalls; ++i) {
        bool isHorizontal = std::rand() % 2;
        int length = 3 + (std::rand() % 4);

        if (isHorizontal) { // установка горизонтальной стены
            int y = std::rand() % height;
            int xStart = std::rand() % (width - length + 1);
            for (int x = xStart; x < xStart + length && x < width; ++x) {
                if (x < 4 && y < 4) continue; // чтобы игрок не встрял
                field[y][x].setIsPassable(false); // стена
            }
        } else {  // установка вертикальной стены
            int x = std::rand() % width;
            int yStart = std::rand() % (height - length + 1);
            for (int y = yStart; y < yStart + length && y < height; ++y) {
                if (x < 4 && y < 4) continue;
                field[y][x].setIsPassable(false); // стена
            }
        }
    }
}

// проверка валидности позиции
bool Field::isValidPos(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height && field[y][x].getIsPassable();
}

// конструктор копирования
Field::Field(const Field& other) : width(other.width), height(other.height), field(other.field) {
}

// оператор копирования
Field::Field(Field&& other) noexcept : width(other.width), height(other.height), field(std::move(other.field)) {
    other.width = 0;
    other.height = 0;
}

// конструктор перемещения
Field& Field::operator=(const Field& other) {
    if (this == &other) return *this;
    width = other.width;
    height = other.height;
    field = other.field;
    return *this;
}

// оператор перемещения
Field& Field::operator=(Field&& other) noexcept {
    if (this == &other) return *this;
    width = other.width;
    height = other.height;
    field = std::move(other.field);
    other.width = 0;
    other.height = 0;
    return *this;
}

void Field::clear() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            field[y][x].setIsPassable(true);
        }
    }
}

std::string Field::toString() const {
    std::string result;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result += field[y][x].getIsPassable() ? '.' : '#';
        }
        result += '\n';
    }
    return result;
}
