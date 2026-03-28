#include "GameField.h"


Field::Field(int widthField, int heightField){
    width = std::min(25, widthField);
    height = std::min(25, heightField);
    width = std::max(10, width);
    height = std::max(10, height);
    grid = std::vector<std::vector<Cell>>(height, std::vector<Cell>(width));
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i=0; i<height; i++){
        for (int k=0; k<width; k++){
            if(i == (int)height/2 - 1 && k == (int)width/2 - 1){
                grid[i][k].setType(CellType::Empty);
            } else if(rand() % 7 ==0){
                grid[i][k].setType(CellType::Wall);
            } else {
                grid[i][k].setType(CellType::Empty);
            }
        }
    }
    
}

Field::Field(const Field& other)
    : width(other.width), height(other.height),
    grid(other.height, std::vector<Cell>(other.width)) {
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            grid[i][j] = other.grid[i][j].deepCopy();
}

Field& Field::operator=(const Field& other) {
    if (this == &other)
        return *this;   

    width = other.width;
    height = other.height;
    grid.resize(height);
    for (int i = 0; i < height; ++i) {
        grid[i].resize(width);
        for (int j = 0; j < width; ++j) {
            
            grid[i][j] = other.grid[i][j].deepCopy();
            
        }
    }
    return *this;
}

Field::Field(Field&& other) noexcept: width(other.width), height(other.height), grid(std::move(other.grid)){
    other.width = 0;
    other.height = 0;
    
}


Field& Field::operator=(Field&& other) noexcept{
    if (&other == this){
        return *this;
    };
    width = other.width;
    height = other.height;

    grid = std::move(other.grid);

    other.width = 0;
    other.height = 0;
    return *this;
}

Field::~Field() {
    width = 0;
    height = 0;
    grid.clear();
}


CellType Field::getCellType(int x, int y) const {
    return grid[y][x].getType();
}

bool Field::canMoveTo(int x, int y) const {
    return x >= 0 && y >= 0 && y < height && x < width
    && !grid[y][x].isOccupated();

}

void Field::setCellType(CellType type, int x, int y) {
    grid[y][x].setType(type);
}

int Field::setCellEntity(GameEnt* entity, int x, int y){
     if (x < 0 || x >= width || y < 0 || y >= height)
        return -1;
    if (grid[y][x].isOccupated()){
        return 0;
    }
    grid[y][x].setEntity(entity);
    return 1;
}

GameEnt* Field::getCellEntity(int x, int y) const {
    if (x < 0 || y < 0 || y >= height || x >= width) return nullptr;
    return grid[y][x].getEntity();
}

int Field::clearCellEnt(int x, int y){
    if (x < 0 || y < 0 || x >= width || y >= height) return 0;
    grid[y][x].clearEnt();
    return 1;
}

int Field::clearCellType(int x, int y){
    if (x < 0 || y < 0 || x >= width || y >= height) return 0;
    grid[y][x].clearType();
    return 1;
}

void Field::printCellInfo(int x, int y) const {
    grid[y][x].printInfo(x, y);
}

bool Field::cellIsEmpty(int x, int y) const {
    return getCellType(x,y)==CellType::Empty;
}

void Field::clearAllEntities() {
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            grid[y][x].clearEnt();
}

//save and load
void Field::save(std::ostream& out) const {
    out << width << ' ' << height << '\n';
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            out << static_cast<int>(grid[y][x].getType()) << ' ';
        }
        out << '\n';
    }
}

void Field::load(std::istream& in) {
    int newWidth = 0;
    int newHeight = 0;
    in >> newWidth >> newHeight;
    if (!in || newWidth <= 0 || newHeight <= 0) {
        throw InvalidSaveDataException("Некорректный размер поля");
    }

    width = newWidth;
    height = newHeight;
    grid.assign(height, std::vector<Cell>(width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int typeInt = 0;
            in >> typeInt;
            if (!in || typeInt < 0 || typeInt > 2) {
                throw InvalidSaveDataException("Ошибка чтения типа клетки поля");
            }
            CellType type = static_cast<CellType>(typeInt);
            grid[y][x].setType(type);
            grid[y][x].clearEnt();
        }
    }
}