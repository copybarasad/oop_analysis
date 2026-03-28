#include "field.h"

Field::Field(int width, int height)
    : width(width), height(height) {
    
    if (width < 10 || height < 10 || width > 25 || height > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }

    grid.resize(height);
    for (int i=0; i < height; i++) {
        grid[i].resize(width);
        for (int j=0; j < width; j++) {
            grid[i][j] = new Cell();
        }
    }
    generateWalls();
}

Field::~Field() {
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            delete grid[i][j];
        }
    }
}

Field::Field(const Field& obj): width(obj.width), height(obj.height) {
    grid.resize(height);
    for (int i=0; i < height; i++) {
        grid[i].resize(width);
        for (int j=0; j < width; j++) {
            if (obj.grid[i][j] != nullptr) {
                grid[i][j] = new Cell(*obj.grid[i][j]);
            } else {
                grid[i][j] = nullptr;
            }
        }
    }
}

Field& Field::operator=(const Field& obj) {
    if (this != &obj) {
        int old_height = height;
        int old_width = width;
        
        width = obj.width;
        height = obj.height;
        
        for (int i = 0; i < old_height; ++i) {
            for (int j = 0; j < old_width; ++j) {
                delete grid[i][j];
            }
        }
        
        grid.resize(height);
        for (int i = 0; i < height; ++i) {
            grid[i].resize(width);
            for (int j = 0; j < width; ++j) {
                grid[i][j] = new Cell(*obj.grid[i][j]);
            }
        }
    }
    return *this;
}
Field::Field(Field&& obj) noexcept
    : width(obj.width), height(obj.height), grid(std::move(obj.grid)) {
    obj.width = 0;
    obj.height = 0;
}

Field& Field::operator=(Field&& obj) noexcept {
    if (this != &obj) {
        for (auto& row : grid) {
            for (auto cell : row) {
                delete cell;
            }
        }

        width = obj.width;
        height = obj.height;
        grid = std::move(obj.grid);

        obj.height = 0;
        obj.width = 0;
    }
    return *this;
    
}

int Field::getWidth() const { return width; }
int Field::getHeight() const { return height; }


const Cell* Field::getCell(int x, int y) const { 
    if (!isValidPosition(x, y)) {
        return nullptr;
    }
    return grid[y][x]; 
}

int Field::isSafeWallPosition(int x, int y) const {
    const Cell* cell = getCell(x, y);
    if (!cell || !cell->isPassable()) return 0;

    int freeNeighbors_line = 0;

    std::pair<int, int> neighbors_line[4] = {
        {x, y - 1}, {x, y + 1},
        {x - 1, y}, {x + 1, y}
    };

    for (const auto& pos : neighbors_line) {
        if (isValidPosition(pos.first, pos.second) &&
            getCell(pos.first, pos.second)->isPassable()) {
            freeNeighbors_line++;
        }
    }

    return freeNeighbors_line;
}


void Field::generateWalls(){
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist_y(1, height-2);
    std::uniform_int_distribution<int> dist_x(1, width-2);
    
    for (int count = 0; count < (width * height)/8; count++) {
        int y = dist_y(gen);
        int x = dist_x(gen);
        
        if (isSafeWallPosition(x, y) >=3) {
            grid[y][x]->setType(CellType::WALL);
        }
    }
    grid[0][0]->setType(CellType::WALL);
    grid[1][1]->setType(CellType::WALL);
    grid[height-1][0]->setType(CellType::WALL);
    grid[height-2][1]->setType(CellType::WALL);
    grid[0][width-1]->setType(CellType::WALL);
    grid[1][width-2]->setType(CellType::WALL);
    grid[height-1][width-1]->setType(CellType::WALL);
    grid[height-2][width-2]->setType(CellType::WALL);           
}

bool Field::canMoveTo(int x, int y) const{
    if (isValidPosition(x, y) && (getCell(x,y)->getType() != CellType::WALL)) {
        return true;
    }
    return false;
}

bool Field::isValidPosition(int x, int y) const {
    if (x >= 0 && x < width && y >=0 && y < height){
        return true;
    }
    return false;
}

void Field::setTrapCell(int x, int y,int dmg, CellType newType){
    if (isValidPosition(x, y)) {
        grid[y][x]->setTrapDamage(dmg);
        grid[y][x]->setType(newType);
    }
}

std::vector<std::tuple<int, int, CellType, int>> Field::getCellsData() const {
    std::vector<std::tuple<int, int, CellType, int>> data;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Cell* cell = grid[y][x];
            data.emplace_back(x, y, cell->getType(), cell->getTrapDamage());
        }
    }
    return data;
}

void Field::loadCellsData(const std::vector<std::tuple<int, int, CellType, int>>& data) {
    for (const auto& cellData : data) {
        int x, y, trapDamage;
        CellType type;
        std::tie(x, y, type, trapDamage) = cellData;
        if (isValidPosition(x, y) && trapDamage >= 0) {
            grid[y][x]->setType(type);
            grid[y][x]->setTrapDamage(trapDamage);
        }else{
            throw std::invalid_argument("Incorrect initialization of the cell:" + std::to_string(x) + 
            ' ' + std::to_string(y));
        }
    }
}